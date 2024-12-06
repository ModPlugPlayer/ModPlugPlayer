/*
ModulePlayer class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ModulePlayer.hpp"
#include <QDebug>
#include <cmath>
#include <MathUtil.hpp>
#include <libopenmpt/libopenmpt.h>
#include <MPPExceptions.hpp>
#include "Util/ModPlugPlayerUtil.hpp"

ModulePlayer::ModulePlayer() {
}

ModulePlayer::~ModulePlayer() {

}

void ModulePlayer::stop() {
    if(!isSongState(SongState::Loaded))
        return;
    if(!isPlayerState(PlayerState::Stopped)) {
        stopStream();
        setPlayerState(PlayerState::Stopped);
    }
}

void ModulePlayer::play() {
    if(!isSongState(SongState::Loaded))
        return;
    if(isPlayerState(PlayerState::Stopped)) {
        playStream();
        setPlayerState(PlayerState::Playing);
    }
    else if(isPlayerState(PlayerState::Paused)) {
        resumeStream();
        setPlayerState(PlayerState::Playing);
    }
}

void ModulePlayer::pause() {
    if(!isSongState(SongState::Loaded))
        return;
    if(isPlayerState(PlayerState::Playing)) {
        pauseStream();
        setPlayerState(PlayerState::Paused);
    }
    else if(isPlayerState(PlayerState::Paused)) {
        resumeStream();
        setPlayerState(PlayerState::Playing);
    }
}

void ModulePlayer::load(const std::filesystem::path filePath) {
    if(!isPlayerState(PlayerState::Stopped)) {
        stopStream();
    }
    ModuleFileInfo moduleFileInfo = initialize(filePath, 2048, 1024, SampleRate::Hz44100);
    if(std::filesystem::exists(filePath)) {
        qDebug()<<filePath.c_str()<<" Loaded";
    }
    if(isPlayerState(PlayerState::Playing)) {
        play();
        qDebug()<<"Playing";
        setPlayerState(PlayerState::Playing);
    }
    else
        setPlayerState(PlayerState::Stopped);
    setSongState(SongState::Loaded);
    emit moduleFileLoaded(moduleFileInfo, true);
}

void ModulePlayer::load(const PlayListItem playListItem) {
    load(playListItem.filePath);
}

void ModulePlayer::getModuleInfo(const std::filesystem::path filePath) {

}

void ModulePlayer::getModuleInfo(const PlayListItem playListItem) {

}

void ModulePlayer::getCurrentModuleInfo() {

}

void logModInfo(const openmpt::module *mod) {
    int NumOfOrders = mod->get_num_orders();
    qDebug()<<"Duration: "<<mod->get_duration_seconds() <<'\n'
           <<"Number of orders: "<< NumOfOrders<<'\n';
    for(int i=0; i<NumOfOrders; i++) {
        qDebug()<<"Number of rows in order "<<i<<" is "<<mod->get_pattern_num_rows(mod->get_order_pattern(i));
    }
}

void logCurrentModState(const openmpt::module *mod){
    qDebug()<<"Speed: "<<mod->get_current_speed()<<" Tempo: "<< mod->get_current_tempo()
         <<" Order: "<<mod->get_current_order()<<" Pattern: "<< mod->get_current_pattern()
        <<" Row: "<<mod->get_current_row() <<" Duration: "<<mod->get_duration_seconds()
       <<" Position time: "<<mod->get_position_seconds();
}

TimeInfo ModulePlayer::getTimeInfo() {
    TimeInfo timeInfo;
    timeInfo.globalRowIndex = 0;
    if(mod != nullptr) {
        int order = mod->get_current_order();
        int row = mod->get_current_row();
        Row &r = rowsByOrders[order][row];
        if(r.time == -1) {
            r.time = mod->get_position_seconds();
        }
        timeInfo.seconds = r.time;
        timeInfo.globalRowIndex = r.rowGlobalIndex;
    }
    return timeInfo;
}

time_t time_since_epoch() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}

void ModulePlayer::timeInfoRequested() {
    if(mod != nullptr)
//        this->sendTimeInfo();
        return;
}

void ModulePlayer::openStream() {
    portaudio::System &sys = portaudio::System::instance();
    portaudio::Device &outputDevice = (outputDeviceIndex < 0) ? sys.defaultOutputDevice() : sys.deviceByIndex(outputDeviceIndex);

    portaudio::DirectionSpecificStreamParameters outputstream_parameters(outputDevice, 2, portaudio::FLOAT32, false, portaudio::System::instance().defaultOutputDevice().defaultHighOutputLatency(), nullptr);
    portaudio::StreamParameters stream_parameters( portaudio::DirectionSpecificStreamParameters::null(), outputstream_parameters, (double) sampleRate, framesPerBuffer, paNoFlag );

    stream.open(stream_parameters, *this, &ModulePlayer::read);
}

ModuleFileInfo ModulePlayer::initialize(const std::filesystem::path filePath, const std::size_t bufferSize, const int framesPerBuffer, const SampleRate sampleRate) {
    ModuleFileInfo moduleFileInfo;
    this->sampleRate = sampleRate;
    this->frequencySpacing = double(sampleRate)/(fftPrecision-1);
    std::vector<OctaveBand<double>> bands = BandFilter<double>::calculateOctaveBands(OctaveBandBase::Base2, 3);
    spectrumAnalyzerBands = SpectrumAnalyzerBands<double>(bands);
    this->bufferSize = bufferSize;
    this->framesPerBuffer = framesPerBuffer;
	qDebug()<<"bar amount"<<spectrumAnalyzerBarAmount;
	spectrumData.assign(spectrumAnalyzerBarAmount,0);
    setSpectrumAnalyzerWindowFunction(spectrumAnalyzerWindowFunction);
    #ifdef Q_OS_MACOS
        if(fftPlan == nullptr) {
            fftInput = fftw_alloc_real(maxBufferSize);
            fftOutput = fftw_alloc_complex(fftPrecision);
        }
        else {
            fftw_destroy_plan(fftPlan);
            fftw_cleanup();
        }
    #else
        if(fftPlan != nullptr) {
            fftw_destroy_plan(fftPlan);
            fftw_free(fftInput);
            fftw_free(fftOutput);
            fftw_cleanup();
        }
        fftInput = fftw_alloc_real(bufferSize);
        fftOutput = fftw_alloc_complex(fftPrecision);
    #endif

    fftPlan = fftw_plan_dft_r2c_1d(2*fftPrecision-1, fftInput, fftOutput, FFTW_MEASURE);
// 2n-1 for example, for 12 fftplan would be 23

    if (!fftPlan)
       qDebug("plan not created");

    try {
        if(leftSoundChannelData != nullptr)
            delete leftSoundChannelData;
        leftSoundChannelData = new float[bufferSize];
        if(rightSoundChannelData != nullptr)
            delete rightSoundChannelData;
        rightSoundChannelData = new float[bufferSize];
        std::fill(leftSoundChannelData, leftSoundChannelData+bufferSize, 0);
        std::fill(rightSoundChannelData, rightSoundChannelData+bufferSize, 0);
        std::ifstream file(filePath, std::ios::binary );
        //stop();

        if(mod != nullptr)
            delete mod;
        try{
            mod = new openmpt::module( file );
        }
        catch(openmpt::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw ModPlugPlayer::Exceptions::UnsupportedFileFormatException();
        }

        mod->ctl_set_boolean("seek.sync_samples", true);
        ModPlugPlayerUtil::Catalog::setAmigaEmulationType(mod, amigaFilter);
		//std::string a = mod->ctl_get("render.resampler.emulate_amiga_type");
        //qDebug()<<"amiga type"<< QString::fromStdString(a);
        ModPlugPlayerUtil::Catalog::setInterpolationFilter(mod, interpolationFilter);

        mod->set_repeat_count((std::int32_t) repeatMode);
        this->rows.clear();
        int numOfOrders = mod->get_num_orders();
        int totalRowAmount = 0;

        for(int i=0; i<numOfOrders; i++) {
            int rowAmountOfCurrentOrder = mod->get_pattern_num_rows(mod->get_order_pattern(i));
            totalRowAmount += rowAmountOfCurrentOrder;
            for(int j=0; j<rowAmountOfCurrentOrder; j++){
                Row r;
                r.rowIndex = j;
                r.orderIndex = i;
                r.rowGlobalIndex = rows.size();
                r.time = -1;
                rows.push_back(r);
            }
        }

        rowsByOrders.clear();
        rowsByOrders.reserve(numOfOrders);
        for(int i=0; i<numOfOrders; i++){
            std::vector<Row> v;
            rowsByOrders.push_back(v);
        }

        for(Row &r : rows){
            rowsByOrders[r.orderIndex].push_back(r);
        }
        moduleFileInfo = ModPlugPlayerUtil::createModuleFileInfoObject(mod, filePath);
        ModPlugPlayerUtil::Catalog::setSongEndBehavior(mod, ModPlugPlayerUtil::Catalog::SongEndBehavior::Continue);

        this->filePath = filePath;

        emit(timeTicksAmountChanged(rows.size()));
        //portaudio::AutoSystem portaudio_initializer;
        openStream();
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "Out of memory" ) << std::endl;
        throw ModPlugPlayer::Exceptions::OutOfMemoryException();
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "Unknown error" ) << std::endl;
        throw ModPlugPlayer::Exceptions::UnknownErrorException();
    }
    return moduleFileInfo;
}

void ModulePlayer::updateFFT() {
    double magnitude;
    //double magnitude_dB;
    spectrumAnalyzerBands.resetMagnitudes();
    soundDataMutex.lock();
    if(spectrumAnalyzerWindowFunction == WindowFunction::None) {
        for (unsigned int i = 0; i < lastReadCount; i++) {
            fftInput[i] = (leftSoundChannelData[i] + rightSoundChannelData[i])/2;
        }
    }
    else {
        for (unsigned int i = 0; i < lastReadCount; i++) {
            fftInput[i] = (leftSoundChannelData[i] + rightSoundChannelData[i])/2 * windowMultipliers[i];
        }
    }
    soundDataMutex.unlock();
    fftw_execute(fftPlan); /* repeat as needed */

    for(int i=0; i<fftPrecision; i++){
        magnitude = DSP::DSP<double>::calculateMagnitude(fftOutput[i][REAL], fftOutput[i][IMAG]);
        //qDebug()<<"magnitude: "<<magnitude;
        SpectrumAnalyzerBandDTO<double> & spectrumAnalyzerBand = spectrumAnalyzerBands[i*frequencySpacing];
        if(spectrumAnalyzerBand.bandInfo.nominalMidBandFrequency >= 0 && !std::isnan(magnitude)){
            spectrumAnalyzerBand.addMagnitude(magnitude);
        }
        //else
        //    qDebug()<<"nan magnitude";
        //spectrumData[i] = DSP<double>::calculateMagnitudeDb(fftOutput[i][REAL], fftOutput[i][IMAG]);
        //qDebug()<<"Max Magnitude: "<<maxMagnitude<<" FFT Output["<<i<<"] Real: "<<QString::number(fftOutput[i][REAL], 'g', 6) << "Imaginary: "<<fftOutput[i][IMAG]<<" Magnitude: "<<magnitude<<" DB: "<<magnitude_dB;
    }
}

PaDeviceIndex ModulePlayer::getOutputDeviceIndex() const {
    return outputDeviceIndex;
}

void ModulePlayer::setOutputDeviceIndex(const PaDeviceIndex newOutputDeviceIndex) {
    outputDeviceIndex = newOutputDeviceIndex;
}

void ModulePlayer::setSpectrumAnalyzerWindowFunction(const WindowFunction windowFunction) {
    soundDataMutex.lock();
    this->spectrumAnalyzerWindowFunction = windowFunction;
    if(windowMultipliers != nullptr) {
        delete[] windowMultipliers;
    }
    switch(windowFunction) {
    case WindowFunction::None:
        windowMultipliers = nullptr;
        break;
    case WindowFunction::HanningWindow:
        windowMultipliers = DSP::DSP<float>::hanningMultipliers(this->framesPerBuffer);
        break;
    case WindowFunction::HammingWindow:
        windowMultipliers = DSP::DSP<float>::hammingMultipliers(this->framesPerBuffer);
        break;
    case WindowFunction::BlackmanWindow:
        windowMultipliers = DSP::DSP<float>::blackmanMultipliers(this->framesPerBuffer);
        break;
    }
    soundDataMutex.unlock();
}

void ModulePlayer::setInterpolationFilter(const InterpolationFilter interpolationFilter) {
    this->interpolationFilter = interpolationFilter;
    if(mod != nullptr)
        ModPlugPlayerUtil::Catalog::setInterpolationFilter(mod, interpolationFilter);
}

void ModulePlayer::setAmigaFilter(const AmigaFilter amigaFilter) {
    this->amigaFilter = amigaFilter;
    if(mod != nullptr)
        ModPlugPlayerUtil::Catalog::setAmigaEmulationType(mod, amigaFilter);
}

RepeatMode ModulePlayer::getRepeatMode() const
{
    return repeatMode;
}

void ModulePlayer::setRepeatMode(const RepeatMode &value) {
    repeatMode = value;
    if(mod != nullptr)
        ModPlugPlayerUtil::setRepeatMode(mod, repeatMode);
}

bool ModulePlayer::getRepeatMode(const RepeatMode &repeatMode) {
    return (this->repeatMode == repeatMode);
}

PlayerState ModulePlayer::getPlayerState() const
{
    return playerState;
}

void ModulePlayer::setPlayerState(const PlayerState &value) {
    playerState = value;
	emit playerStateChanged(value);
}

bool ModulePlayer::isPlayerState(const PlayerState &playerState) {
    return (this->playerState == playerState);
}

SongState ModulePlayer::getSongState() const {
    return songState;
}

void ModulePlayer::setSongState(const SongState &value) {
    songState = value;
	emit songStateChanged(value);
}

bool ModulePlayer::isSongState(const SongState &songState) {
    return this->songState == songState;
}

int ModulePlayer::read(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo, const PaStreamCallbackFlags statusFlags){
    assert(outputBuffer != nullptr);

    float **out = static_cast<float **>(outputBuffer);

    soundDataMutex.lock();
    lastReadCount = mod->read((std::int32_t) sampleRate, (std::size_t) framesPerBuffer, leftSoundChannelData, rightSoundChannelData);
    soundDataMutex.unlock();

    for(unsigned int i = 0; i < lastReadCount; i++) {
        if (lastReadCount == 0) {
            break;
        }
        try {
            out[0][i] = leftSoundChannelData[i]*volume;
            //qDebug()<<out[0][i];
            out[1][i] = rightSoundChannelData[i]*volume;

            //const float * const buffers[2] = { left.data(), right.data() };
            //stream.write( buffers, static_cast<unsigned long>( count ) );
        } catch ( const portaudio::PaException & pa_exception ) {
            if ( pa_exception.paError() != paOutputUnderflowed ) {
                throw;
            }
        }
    }

    //emit spectrumAnalyzerData(10, magnitude);


    //qDebug()<<(int)(magnitude[0]*100)<<"\t"<<(int)(magnitude[1]*100)<<"\t"<<100*magnitude[2]<<"\t"<<magnitude[3]<<"\t"<<magnitude[4]<<"\t"<<magnitude[5]<<"\t"<<magnitude[6]<<"\t"<<magnitude[7]<<"\t"<<magnitude[8]<<"\t"<<magnitude[9];
    //qDebug()<<"Count: "<<count;


    if(lastReadCount==0) {
        if(repeatMode == RepeatMode::NoRepeat){
            stop();
            return PaStreamCallbackResult::paComplete;
        }
        if(repeatMode == RepeatMode::RepeatTrack)
            mod->set_position_seconds(0);
        //if(repeatMode == RepeatMode::LoopTrack)
            //mod->set_position_seconds(0);

    }

    return PaStreamCallbackResult::paContinue;
}

int ModulePlayer::closeStream() {
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput); fftw_free(fftOutput);
}

int ModulePlayer::playStream() {
    try {
        if(!stream.isOpen())
            openStream();
        logModInfo(mod);
        stream.start();

        /* when this block runs, it freezes the player window.
        while(stream.isActive())
            portaudio::System::instance().sleep(100);

        stream.stop();
        qDebug()<<"Stopped";

        stream.close();
        */
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "out of memory" ) << std::endl;
        return 1;
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "unknown error" ) << std::endl;
        return 1;
    }
    return 0;
}

int ModulePlayer::stopStream() {
    if(stream.isActive()){
        stream.stop();
        qDebug()<<"Stream has been stopped";
        scrubTime(0);
    }
    /*
    if(stream.isOpen()) {
        stream.close();
        qDebug()<<"Stream has been closed";
    }*/
    return 0;
}

int ModulePlayer::pauseStream() {
    qDebug()<<"Stream has been paused";
    stream.stop();
	return 0;
}

int ModulePlayer::resumeStream() {
    stream.start();
	return 0;
}

std::string ModulePlayer::getSongTitle() {
    return mod->get_metadata("title");
}

std::filesystem::path ModulePlayer::getFilePath() {
    return filePath;
}

std::vector<std::string> ModulePlayer::getSupportedExtensions() {
    return openmpt::get_supported_extensions();
}

size_t ModulePlayer::getSongDuration() {
    return ModPlugPlayerUtil::getSongDuration(mod);
}

void ModulePlayer::scrubTime(const int rowGlobalId) {
    Row r = rows[rowGlobalId];
    mod->set_position_order_row(r.orderIndex, r.rowIndex);

    //double seconds = mod->get_duration_seconds()/(double)rows.size()*(double)rowGlobalId;
    //mod->set_position_seconds(seconds);
}

void ModulePlayer::setVolume(const double volume) {
    this->volume = volume;
}

void ModulePlayer::getSpectrumData(double * spectrumData) {
    if(playerState == PlayerState::Playing) {
        updateFFT();
        this->spectrumAnalyzerBands.getAmplitudes(spectrumData, 24);
    }
    else
        std::fill(spectrumData, spectrumData+20, 0);
}

float ModulePlayer::getVuMeterValue() {
	if(playerState == PlayerState::Playing) {
		float value;
		soundDataMutex.lock();
        value = DSP::DSP<float>::calculateVolumeDbLevel(leftSoundChannelData, rightSoundChannelData, framesPerBuffer);
		soundDataMutex.unlock();
		return value;
	}
	else
		return -INFINITY;
}
