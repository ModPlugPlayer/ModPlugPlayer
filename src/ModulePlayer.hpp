/*
ModulePlayer class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MODULEPLAYER_HPP
#define MODULEPLAYER_HPP

#include <QObject>
#include <mutex>
#include <QTimer>
#include <exception>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>
#include <vector>

#include <libopenmpt/libopenmpt.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include "Enums.hpp"
#include "MppParameters.hpp"
#include "ModuleClasses.hpp"
#include <fftw3.h>
#include <BandFilter.hpp>
#include <DSP.hpp>
#include <filesystem>
#include <APIStructures.hpp>

using namespace ModPlugPlayer;

class ModulePlayer:public QObject
{
    Q_OBJECT
public:
    ModulePlayer();
    ~ModulePlayer();

    std::string getSongTitle();
    std::filesystem::path getFilePath();
    std::vector<std::string> getSupportedExtensions();
    size_t getSongDuration();

    portaudio::StreamParameters streamParameters;
    portaudio::DirectionSpecificStreamParameters outputSpecificStreamParameters;
//    portaudio::BlockingStream stream;
    portaudio::MemFunCallbackStream<ModulePlayer> stream;
    int read(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
    TimeInfo getTimeInfo();
    void scrubTime(int rowGlobalId);
    void setVolume(double volume);
    void getSpectrumData(double * spectrumData);
    float getVuMeterValue();

    SongState getSongState() const;
    void setSongState(const SongState &value);
    bool isSongState(const SongState &songState);

    PlayerState getPlayerState() const;
    void setPlayerState(const PlayerState &value);
    bool isPlayerState(const PlayerState &playerState);

    RepeatState getRepeatState() const;
    void setRepeatState(const RepeatState &value);
    bool isRepeatState(const RepeatState &repeatState);
    PaDeviceIndex getOutputDeviceIndex() const;
    void setOutputDeviceIndex(PaDeviceIndex newOutputDeviceIndex);
    void setSpectrumAnalyzerWindowFunction(WindowFunction windowFunction);

signals:
    void timeChanged(TimeInfo timeInfo);
    void timeTicksAmountChanged(int amount);
    void spectrumAnalyzerData(int amount, double *magnitudes);
	void playerStateChanged(PlayerState playerState);
	void songStateChanged(SongState songState);
	void repeatStateChanged(RepeatState repeatState);
    void resultReady(const QString &s);
    void fileOpened();
public slots:
    void timeInfoRequested();
    void stop();
    void play();
    void pause();
    void open(std::filesystem::path filePath);

private:
    std::filesystem::path filePath;
    size_t spectrumAnalyzerBarAmount = 20;
    openmpt::module *mod = nullptr;
    SampleRate sampleRate;
    double frequencySpacing;
    int fftPrecision = 1024;
    SpectrumAnalyzerBands<double> spectrumAnalyzerBands;
    std::size_t bufferSize = 1024;
    size_t framesPerBuffer = 512;
    float *leftSoundChannelData = nullptr, *rightSoundChannelData = nullptr;
    size_t lastReadCount = 0;
    std::vector<Row> rows;
    std::vector<std::vector<Row>> rowsByOrders;
    void sendTimeInfo();
    double volume = 0;
    fftw_plan fftPlan = nullptr;
    double *fftInput;
    fftw_complex *fftOutput;
    float *windowMultipliers = nullptr;
    float maxMagnitude = 0;
    std::vector<double> spectrumData;
    std::timed_mutex soundDataMutex;
    void updateFFT();
    PlayerState playerState = PlayerState::Stopped;
    SongState songState = SongState::NotLoaded;
    RepeatState repeatState = RepeatState::SingleTrack;
    PaDeviceIndex outputDeviceIndex = -1;
    WindowFunction spectrumAnalyzerWindowFunction = WindowFunction::None;

    void openStream();
    int initialize(std::filesystem::path filePath, std::size_t bufferSize, int framesPerBuffer, SampleRate sampleRate = SampleRate::Hz48000);
    int closeStream();
    int playStream();
    int stopStream();
    int pauseStream();
    int resumeStream();
};

#endif // MODULEPLAYER_HPP
