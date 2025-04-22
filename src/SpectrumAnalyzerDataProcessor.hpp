/*
SpectrumAnalyzerDataProcessor class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include <QObject>
#include <BandFilter.hpp>
#include <Interfaces/FFT.hpp>
#include <APIStructures.hpp>

using namespace ModPlugPlayer;

class SpectrumAnalyzerDataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit SpectrumAnalyzerDataProcessor(std::timed_mutex &soundDataMutex);
    ~SpectrumAnalyzerDataProcessor();
    void initalize(size_t bufferSize, size_t framesPerBuffer);
    void calculateSpectrumData(size_t readCount, float *leftSoundChannelData, float *rightSoundChannelData, double *spectrumData);
    void close();

private:
    double frequencySpacing = 0;
    int fftPrecision = 0;
    size_t spectrumAnalyzerBarAmount = 20;
    SpectrumAnalyzerBands<double> spectrumAnalyzerBands;
    Interfaces::FFT<float> *fft = nullptr;
    SoundResolution soundResolution;
    std::timed_mutex &soundDataMutex;
    float *windowMultipliers = nullptr;
    WindowFunction windowFunction = WindowFunction::None;
    size_t bufferSize = 0;
    size_t framesPerBuffer = 0;
    void updateFFT(size_t inputDataCount, float *leftSoundChannelData, float *rightSoundChannelData, double *spectrumData);
    void connectSignalsAndSlots();
    void setWindowFunction(const WindowFunction windowFunction);
private slots:
    void onSoundResolutionChanged(const SoundResolution soundResolution);
    void onWindowFunctionChangeRequested(const ModPlugPlayer::WindowFunction windowFunction);
};
