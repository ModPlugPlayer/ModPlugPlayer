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

class ModulePlayer:public QObject
{
    Q_OBJECT
public:
    ModulePlayer();
    void run();
    int open(std::string fileName, std::size_t bufferSize, int framesPerBuffer, SAMPLERATE sampleRate = SAMPLERATE::Hz48000);
    int play();
    int stop();
    int pause();
    int resume();

    portaudio::StreamParameters streamParameters;
    portaudio::DirectionSpecificStreamParameters outputSpecificStreamParameters;
//    portaudio::BlockingStream stream;
    portaudio::MemFunCallbackStream<ModulePlayer> stream;
    int read(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
    TimeInfo getTimeInfo();
    void scrubTime(int rowGlobalId);
    void setVolume(double volume);
signals:
    void timeChanged(TimeInfo timeInfo);
    void timeTicksAmountChanged(int amount);
public slots:
    void mppParametersChanged(MppParameters &mppParameters);
    void timeInfoRequested();

private:
    openmpt::module *mod;
    SAMPLERATE sampleRate;
    std::size_t bufferSize;
    int framesPerBuffer;
    std::vector<float> left, right;
    MppParameters mppParameters;
    std::vector<Row> rows;
    std::vector<std::vector<Row>> rowsByOrders;
    void sendTimeInfo();
    double volume;
};

#endif // MODULEPLAYER_HPP
