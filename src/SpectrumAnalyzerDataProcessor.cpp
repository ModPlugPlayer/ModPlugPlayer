/*
SpectrumAnalyzerDataProcessor class declarations of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "SpectrumAnalyzerDataProcessor.hpp"
//#include "Implementation/FFT/KissFFTImpl.hpp"
#include "Implementation/FFT/FFTWImpl.hpp"
#include <MessageCenter.hpp>
#include "SettingsCenter.hpp"

SpectrumAnalyzerDataProcessor::SpectrumAnalyzerDataProcessor(std::timed_mutex &soundDataMutex)
    : QObject{nullptr}, soundDataMutex(soundDataMutex) {
    //fft = new KissFFTImpl<float>();
    fft = new FFTWImpl<float>();
    connectSignalsAndSlots();
}

SpectrumAnalyzerDataProcessor::~SpectrumAnalyzerDataProcessor() {
    if(fft == nullptr)
        return;
    if(fft->isOpen())
        fft->close();
    delete fft;
    fft = nullptr;
}

void SpectrumAnalyzerDataProcessor::initalize(size_t bufferSize, size_t framesPerBuffer) {
    this->bufferSize = bufferSize;
    this->framesPerBuffer = framesPerBuffer;
    this->fftPrecision = framesPerBuffer;
    this->frequencySpacing = double(soundResolution.sampleRate)/fftPrecision;
    std::vector<OctaveBand<double>> bands = BandFilter<double>::calculateOctaveBands(OctaveBandBase::Base2, 2);
    spectrumAnalyzerBands = SpectrumAnalyzerBands<double>(bands);
    qDebug()<<"Spectrum analyzer bar amount is"<<spectrumAnalyzerBarAmount;
    //spectrumData->assign(spectrumAnalyzerBarAmount, 0);

    setWindowFunction(SettingsCenter::getInstance().getParameters()->spectrumAnalyzerWindowFunction);

    fft->initialize(framesPerBuffer);
}

void SpectrumAnalyzerDataProcessor::updateFFT(size_t inputDataCount, float *leftSoundChannelData, float *rightSoundChannelData, double *spectrumData) {
    if(spectrumData == nullptr)
        return;
    double magnitude;
    //double magnitude_dB;
    spectrumAnalyzerBands.resetMagnitudes();
    soundDataMutex.lock();
    if(windowFunction == WindowFunction::None) {
        for (unsigned int i = 0; i < inputDataCount; i++) {
            if(fft->fftInput != nullptr)
                fft->fftInput[i] = (leftSoundChannelData[i]/2 + rightSoundChannelData[i]/2);
        }
    }
    else {
        for (unsigned int i = 0; i < inputDataCount; i++) {
            if(fft->fftInput != nullptr)
                fft->fftInput[i] = (leftSoundChannelData[i]/2 + rightSoundChannelData[i]/2) * windowMultipliers[i];
        }
    }
    soundDataMutex.unlock();
    fft->execute();

    for(int i=0; i<fftPrecision; i++) {
        magnitude = AndromedaDSP::AndromedaDSP<double>::calculateMagnitude(fft->fftOutput[i].real(), fft->fftOutput[i].imag());
        //qDebug()<<"magnitude: "<<magnitude;
        SpectrumAnalyzerBandDTO<double> & spectrumAnalyzerBand = spectrumAnalyzerBands[i*frequencySpacing];
        //if(spectrumAnalyzerBand.bandInfo.nominalMidBandFrequency >= 0 && !std::isnan(magnitude)){
        spectrumAnalyzerBand.addMagnitude(magnitude);
        //}
        //else
        //    qDebug()<<"nan magnitude";
        //spectrumData[i] = AndromedaDSP<double>::calculateMagnitudeDb(fftOutput[i][REAL], fftOutput[i][IMAG]);
        //qDebug()<<"Max Magnitude: "<<maxMagnitude<<" FFT Output["<<i<<"] Real: "<<QString::number(fftOutput[i][REAL], 'g', 6) << "Imaginary: "<<fftOutput[i][IMAG]<<" Magnitude: "<<magnitude<<" DB: "<<magnitude_dB;
    }
}

void SpectrumAnalyzerDataProcessor::calculateSpectrumData(size_t inputDataCount, float *leftSoundChannelData, float *rightSoundChannelData, double *spectrumData) {
    //if(playerState == PlayingState::Playing) {
        updateFFT(inputDataCount, leftSoundChannelData, rightSoundChannelData, spectrumData);
        this->spectrumAnalyzerBands.getAmplitudes(spectrumData, 0);
    //}
    //else
    //    std::fill(spectrumData, spectrumData+20, 0);
}

void SpectrumAnalyzerDataProcessor::setWindowFunction(const WindowFunction windowFunction) {
    soundDataMutex.lock();
    this->windowFunction = windowFunction;
    if(windowMultipliers != nullptr) {
        delete[] windowMultipliers;
    }
    switch(windowFunction) {
    case WindowFunction::None:
        windowMultipliers = nullptr;
        break;
    case WindowFunction::HanningWindow:
        windowMultipliers = AndromedaDSP::AndromedaDSP<float>::hanningMultipliers(this->framesPerBuffer);
        break;
    case WindowFunction::HammingWindow:
        windowMultipliers = AndromedaDSP::AndromedaDSP<float>::hammingMultipliers(this->framesPerBuffer);
        break;
    case WindowFunction::BlackmanWindow:
        windowMultipliers = AndromedaDSP::AndromedaDSP<float>::blackmanMultipliers(this->framesPerBuffer);
        break;
    }
    soundDataMutex.unlock();
    emit MessageCenter::getInstance().events.spectrumAnalyzerEvents.windowFunctionChanged(windowFunction);
}

void SpectrumAnalyzerDataProcessor::close() {
    if(fft != nullptr && fft->isOpen())
        fft->close();
}

void SpectrumAnalyzerDataProcessor::connectSignalsAndSlots() {
    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::soundResolutionChanged, this, &SpectrumAnalyzerDataProcessor::onSoundResolutionChanged);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::windowFunctionChangeRequested, this, &SpectrumAnalyzerDataProcessor::onWindowFunctionChangeRequested);
}

void SpectrumAnalyzerDataProcessor::onSoundResolutionChanged(const SoundResolution soundResolution) {
    this->soundResolution = soundResolution;
}

void SpectrumAnalyzerDataProcessor::onWindowFunctionChangeRequested(const WindowFunction windowFunction) {
    setWindowFunction(windowFunction);
}
