/*
ModulePlayer class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <QObject>
#include <mutex>
#include <QTimer>
#include <vector>

#include <libopenmpt/libopenmpt.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>
#include "ModuleClasses.hpp"
#include <BandFilter.hpp>
#include <DSP.hpp>
#include <filesystem>
#include <APIStructures.hpp>
#include <PlayListDTOs.hpp>
#include "Interfaces/FFT.hpp"

using namespace ModPlugPlayer;

class ModuleHandler:public QObject
{
    Q_OBJECT
public:
    ModuleHandler();
    ~ModuleHandler();

    void stop();
    void play();
    void pause();
    void resume();
    void load(const std::filesystem::path filePath);
    void load(const ModPlugPlayer::PlayListItem playListItem);

    std::string getSongTitle();
    std::filesystem::path getFilePath();
    std::vector<std::string> getSupportedExtensions();
    size_t getSongDuration();
    size_t getChannelAmount();
    size_t getActiveChannelAmount();
    size_t getSubSongAmount();
    std::string getModuleFormat();
    size_t getCurrentSubSongIndex();
    size_t getPatternAmount();
    size_t getCurrentPatternIndex();

    portaudio::StreamParameters streamParameters;
    portaudio::DirectionSpecificStreamParameters outputSpecificStreamParameters;
    //    portaudio::BlockingStream stream;
    portaudio::MemFunCallbackStream<ModuleHandler> stream;
    int read(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer,
             const PaStreamCallbackTimeInfo *timeInfo, const PaStreamCallbackFlags statusFlags);
    TimeInfo getTimeInfo();
    unsigned int getCurrentRowGlobalIndex();
    unsigned int getGlobalRowAmount();
    void scrubTime(const int rowGlobalId);
    void setVolume(const double volume);
    void getSpectrumData(double * spectrumData);
    float getVuMeterValue();

    SongState getSongState() const;
    void setSongState(const SongState &value);
    bool isSongState(const SongState &songState);

    PlayingState getPlayerState() const;
    void setPlayerState(const PlayingState &value);
    bool isPlayerState(const PlayingState &playerState);

    RepeatMode getRepeatMode() const;
    void setRepeatMode(const RepeatMode &repeatMode);
    bool getRepeatMode(const RepeatMode &repeatMode);
    PaDeviceIndex getOutputDeviceIndex() const;
    void setOutputDeviceIndex(const PaDeviceIndex newOutputDeviceIndex);
    void setSoundResolution(const SoundResolution soundResolution);
    void setSpectrumAnalyzerWindowFunction(const WindowFunction windowFunction);
    void setInterpolationFilter(const InterpolationFilter interpolationFilter);
    void setAmigaFilter(const AmigaFilter amigaFilter);
signals:
    void timeChanged(const TimeInfo timeInfo);
    void timeTicksAmountChanged(const int amount);
    void spectrumAnalyzerData(const int amount, const double *magnitudes);
    void playerStateChanged(const PlayingState playerState);
    void songStateChanged(const SongState songState);
    void resultReady(const QString &s);
    void moduleFileInfo(const SongFileInfo info);
    void currentModuleFileInfo(const SongFileInfo info);
public slots:
    void timeInfoRequested();
    void getModuleInfo(const std::filesystem::path filePath);
    void getModuleInfo(const ModPlugPlayer::PlayListItem playListItem);
    void getCurrentModuleInfo();
private:
    void onOutputDeviceChangeRequested(const int outputDeviceIndex);
    std::filesystem::path filePath;
    size_t spectrumAnalyzerBarAmount = 20;
    openmpt::module *mod = nullptr;
    SoundResolution soundResolution;
    double frequencySpacing = 0;
    int fftPrecision = 512;
    SpectrumAnalyzerBands<double> spectrumAnalyzerBands;
    std::size_t bufferSize = 1024;
    size_t framesPerBuffer = 512;
    float *leftSoundChannelData = nullptr, *rightSoundChannelData = nullptr;
    size_t lastReadCount = 0;
    std::vector<Row> rows;
    std::vector<std::vector<Row>> rowsByOrders;
    void sendTimeInfo();
    double volume = 0;
    Interfaces::FFT<float> *fft;
    float *windowMultipliers = nullptr;
    float maxMagnitude = 0;
    std::vector<double> spectrumData;
    std::timed_mutex soundDataMutex;
    void updateFFT();
    PlayingState playerState = PlayingState::Stopped;
    InterpolationFilter interpolationFilter = InterpolationFilter::NoInterpolation;
    AmigaFilter amigaFilter = AmigaFilter::Unfiltered;
    SongState songState = SongState::NotLoaded;
    RepeatMode repeatMode = RepeatMode::LoopSong;
    PaDeviceIndex outputDeviceIndex = -1;
    WindowFunction spectrumAnalyzerWindowFunction = WindowFunction::None;
    const std::size_t maxBufferSize = 1024;
    SongFileInfo currentSongFileInfo; //loaded module file info
    PlayListItem currentPlayListItem; //loaded playlist item info
    PlayingMode playingMode = PlayingMode::Song;

    void openStream();
    SongFileInfo initialize(const std::filesystem::path filePath, const std::size_t bufferSize, const int framesPerBuffer);
    int closeStream();
    int playStream();
    int stopStream();
    int pauseStream();
    int resumeStream();
    int resetStream();
};
