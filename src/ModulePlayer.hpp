#ifndef MODULEPLAYER_HPP
#define MODULEPLAYER_HPP

#include <QObject>
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

class ModulePlayer:public QObject
{
    Q_OBJECT
public:
    ModulePlayer();
    void run();

    std::string getSongTitle();
    std::filesystem::path getFilePath();
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
    void open(QString filePath);

private:
    std::filesystem::path filePath;
    size_t spectrumAnalyzerBarAmount = 20;
    openmpt::module *mod = nullptr;
    SampleRate sampleRate;
    double frequencySpacing;
    int fftPrecision = 1024;
    SpectrumAnalyzerBands<double> spectrumAnalyzerBands;
    std::size_t bufferSize;
    size_t framesPerBuffer;
    float *left = nullptr, *right = nullptr;
    size_t lastReadCount = 0;
    std::vector<Row> rows;
    std::vector<std::vector<Row>> rowsByOrders;
    void sendTimeInfo();
    double volume;
    void openStream();
    fftw_plan fftPlan = nullptr;
    double *fftInput;
    fftw_complex *fftOutput;
    float *hanningMultipliers;
    float maxMagnitude = 0;
    std::vector<double> spectrumData;
    std::timed_mutex soundDataMutex;
    void updateFFT();
    PlayerState playerState = PlayerState::Stopped;
    SongState songState = SongState::NotLoaded;
    RepeatState repeatState = RepeatState::RepeatForewer;

    int openStream(std::string fileName, std::size_t bufferSize, int framesPerBuffer, SampleRate sampleRate = SampleRate::Hz48000);
    int closeStream();
    int playStream();
    int stopStream();
    int pauseStream();
    int resumeStream();

};

#endif // MODULEPLAYER_HPP
