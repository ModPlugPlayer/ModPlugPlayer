#include "ModulePlayer.hpp"
#include <QDebug>
#include <cmath>
#include <MathUtil.hpp>

void ModulePlayer::mppParametersChanged(MppParameters &mppParameters) {
    this->mppParameters.update(mppParameters);
}

void logModInfo(openmpt::module *mod) {
    int NumOfOrders = mod->get_num_orders();
    qDebug()<<"Duration: "<<mod->get_duration_seconds()<<endl
           <<"Number of orders: "<< NumOfOrders<<endl;
    for(int i=0; i<NumOfOrders; i++) {
        qDebug()<<"Number of rows in order "<<i<<" is "<<mod->get_pattern_num_rows(mod->get_order_pattern(i));
    }
}

void logCurrentModState(openmpt::module *mod){
    qDebug()<<"Speed: "<<mod->get_current_speed()<<" Tempo: "<< mod->get_current_tempo()
         <<" Order: "<<mod->get_current_order()<<" Pattern: "<< mod->get_current_pattern()
        <<" Row: "<<mod->get_current_row() <<" Duration: "<<mod->get_duration_seconds()
       <<" Position time: "<<mod->get_position_seconds();
}

TimeInfo ModulePlayer::getTimeInfo(){
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

time_t time_since_epoch()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}

void ModulePlayer::timeInfoRequested(){
    if(mod != nullptr)
//        this->sendTimeInfo();
        return;
}

void ModulePlayer::openStream() {
        portaudio::DirectionSpecificStreamParameters outputstream_parameters(portaudio::System::instance().defaultOutputDevice(), 2, portaudio::FLOAT32, false, portaudio::System::instance().defaultOutputDevice().defaultLowOutputLatency(), 0 );
        portaudio::StreamParameters stream_parameters( portaudio::DirectionSpecificStreamParameters::null(), outputstream_parameters, sampleRate, framesPerBuffer, paNoFlag );

        stream.open(stream_parameters, *this, &ModulePlayer::read);
}



ModulePlayer::ModulePlayer()
{

}

int ModulePlayer::open(std::string fileName, std::size_t bufferSize, int framesPerBuffer, SampleRate sampleRate){
    this->sampleRate = sampleRate;
    this->frequencySpacing = sampleRate/(fftPrecision-1);
    std::vector<OctaveBand<double>> bands = BandFilter<double>::calculateOctaveBands(OctaveBandBase::Base2, 3);
    spectrumAnalyzerBands = SpectrumAnalyzerBands<double>(bands);
    this->bufferSize = bufferSize;
    this->framesPerBuffer = framesPerBuffer;
    this->hanningMultipliers = DSP::hanningMultipliers<float>(this->framesPerBuffer);
    int barAmount = mppParameters.getBarAmount();
    qDebug()<<"bar amount"<<barAmount;
    spectrumData.assign(barAmount,0);
    fftInput = fftw_alloc_real(bufferSize);
    fftOutput = fftw_alloc_complex(fftPrecision);
// 2n-1 for example, for 12 fftplan would be 23
    fftPlan = fftw_plan_dft_r2c_1d(2*fftPrecision-1, fftInput, fftOutput, FFTW_ESTIMATE);

    if (!fftPlan)
       qDebug("plan not created");

    try {
        left.clear();
        right.clear();
        left.reserve(bufferSize);
        right.reserve(bufferSize);
        std::ifstream file(fileName, std::ios::binary );
        //stop();

        if(mod != nullptr)
            delete mod;
        mod = new openmpt::module( file );

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

        emit(timeTicksAmountChanged(rows.size()));
        //portaudio::AutoSystem portaudio_initializer;
        openStream();
    } catch ( const std::bad_alloc & ) {
        std::cerr << "Error: " << std::string( "out of memory" ) << std::endl;
        return 1;
    } catch ( const std::exception & e ) {
        std::cerr << "Error: " << std::string( e.what() ? e.what() : "unknown error" ) << std::endl;
        return 1;
    }
    return 0;
}

void ModulePlayer::updateFFT(void *outputBuffer, unsigned long framesPerBuffer) {
    fftw_execute(fftPlan); /* repeat as needed */

    double magnitude;
    //double magnitude_dB;
    spectrumDataMutex.lock();
    spectrumAnalyzerBands.resetMagnitudes();
    for(int i=0; i<fftPrecision; i++){
        magnitude = DSP::calculateMagnitude(fftOutput[i][REAL], fftOutput[i][IMAG]);
        //qDebug()<<"magnitude: "<<magnitude;
        SpectrumAnalyzerBandDTO<double> & spectrumAnalyzerBand = spectrumAnalyzerBands[i*frequencySpacing];
        if(!isnan(magnitude)){
            spectrumAnalyzerBand.magnitude += magnitude;
            spectrumAnalyzerBand.sampleAmount++;
        }
        //else
        //    qDebug()<<"nan magnitude";
        //spectrumData[i] = DSP::calculateMagnitudeDb(fftOutput[i][REAL], fftOutput[i][IMAG]);
        //qDebug()<<"Max Magnitude: "<<maxMagnitude<<" FFT Output["<<i<<"] Real: "<<QString::number(fftOutput[i][REAL], 'g', 6) << "Imaginary: "<<fftOutput[i][IMAG]<<" Magnitude: "<<magnitude<<" DB: "<<magnitude_dB;
    }
    spectrumDataMutex.unlock();

}

PlayerState ModulePlayer::getPlayerState() const
{
    return playerState;
}

void ModulePlayer::setPlayerState(const PlayerState &value)
{
    playerState = value;
}

bool ModulePlayer::isPlayerState(const PlayerState &playerState)
{
    return (this->playerState == playerState);
}

SongState ModulePlayer::getSongState() const
{
    return songState;
}

void ModulePlayer::setSongState(const SongState &value)
{
    songState = value;
}

bool ModulePlayer::isSongState(const SongState &songState)
{
    return this->songState == songState;
}

int ModulePlayer::read(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags){
    assert(outputBuffer != NULL);

    float **out = static_cast<float **>(outputBuffer);

    if(mppParameters.isAnyParameterChanged()) {
        if(mppParameters.isRepeatCountChanged()) {
            mod->set_repeat_count(mppParameters.getRepeatCount());
        }
        if(mppParameters.isInterpolationFilterChanged())
            mod->set_render_param(mod ->RENDER_INTERPOLATIONFILTER_LENGTH, mppParameters.getInterpolationFilter());
        mppParameters.clearChangedFlags();
    }

    std::size_t count = mod->read( sampleRate, framesPerBuffer, left.data(), right.data() );
    for (unsigned int i = 0; i < count; i++)
    {
        if ( count == 0 ) {
            break;
        }
        try {
            out[0][i] = left[i]*volume;
            //qDebug()<<out[0][i];
            out[1][i] = right.data()[i]*volume;
            fftInput[i] = ((left[i] + right[i])/2) * hanningMultipliers[i];

            //const float * const buffers[2] = { left.data(), right.data() };
            //stream.write( buffers, static_cast<unsigned long>( count ) );
        } catch ( const portaudio::PaException & pa_exception ) {
            if ( pa_exception.paError() != paOutputUnderflowed ) {
                throw;
            }
        }
    }

    updateFFT(outputBuffer, framesPerBuffer);

        //emit spectrumAnalyzerData(10, magnitude);


    //qDebug()<<(int)(magnitude[0]*100)<<"\t"<<(int)(magnitude[1]*100)<<"\t"<<100*magnitude[2]<<"\t"<<magnitude[3]<<"\t"<<magnitude[4]<<"\t"<<magnitude[5]<<"\t"<<magnitude[6]<<"\t"<<magnitude[7]<<"\t"<<magnitude[8]<<"\t"<<magnitude[9];
    //qDebug()<<"Count: "<<count;

    if(count==0) {
        stop();
        return PaStreamCallbackResult::paComplete;
    }

    return PaStreamCallbackResult::paContinue;
}

int ModulePlayer::close()
{
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput); fftw_free(fftOutput);
}

int ModulePlayer::play() {
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

int ModulePlayer::stop() {
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

int ModulePlayer::pause()
{
    qDebug()<<"Stream has been paused";
    stream.stop();
}

int ModulePlayer::resume()
{
    stream.start();
}

std::string ModulePlayer::getSongTitle()
{
    return mod->get_metadata("title");
}

void ModulePlayer::scrubTime(int rowGlobalId){
    Row r = rows[rowGlobalId];
    mod->set_position_order_row(r.orderIndex, r.rowIndex);

    //double seconds = mod->get_duration_seconds()/(double)rows.size()*(double)rowGlobalId;
    //mod->set_position_seconds(seconds);
}

void ModulePlayer::setVolume(double volume){
    this->volume = volume;
}

void ModulePlayer::getSpectrumData(double * spectrumData)
{
    spectrumDataMutex.lock();
    int i=0;
    for(const SpectrumAnalyzerBandDTO<double> & band : this->spectrumAnalyzerBands.getData()) {
        if(i>23)
            spectrumData[i-23] = band.sampleAmount > 0 ? band.magnitude/band.sampleAmount : 0;
        i++;
    }
    spectrumDataMutex.unlock();
}

void ModulePlayer::run(){
    this->play();
}
