#include "BeeperWIthCallback.hpp"

#include <QDebug>

#include <cmath>
#include <cassert>
#include <cstddef>

#include <libopenmpt/libopenmpt.hpp>

#include <portaudiocpp/PortAudioCpp.hxx>

// Some constants:
const int NUM_SECONDS = 2;
const double SAMPLE_RATE = SAMPLERATE::Hz44100;
const int FRAMES_PER_BUFFER = 64;
const int TABLE_SIZE = 200;

// SineGenerator class:
class SineGenerator
{
public:
    SineGenerator(int tableSize) : tableSize_(tableSize), leftPhase_(0), rightPhase_(0)
    {
        const double PI = 3.14159265;
        table_ = new float[tableSize];
        for (int i = 0; i < tableSize; ++i)
        {
            table_[i] = 0.125f * (float)sin(((double)i/(double)tableSize)*PI*2.0);
        }
    }

    ~SineGenerator()
    {
        delete[] table_;
    }

    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
    {
        assert(outputBuffer != NULL);

        float **out = static_cast<float **>(outputBuffer);

        /*
        std::memcpy(out[0], table_, framesPerBuffer);
        std::memcpy(out[1], table_+1, framesPerBuffer);
        */
        for (unsigned int i = 0; i < framesPerBuffer; ++i)
        {
            out[0][i] = table_[leftPhase_];
            out[1][i] = table_[rightPhase_];

            leftPhase_ += 1;
            if (leftPhase_ >= tableSize_)
                leftPhase_ -= tableSize_;

            rightPhase_ += 3;
            if (rightPhase_ >= tableSize_)
                rightPhase_ -= tableSize_;
        }

        return paContinue;
    }

private:
    float *table_;
    int tableSize_;
    int leftPhase_;
    int rightPhase_;
};


int BeeperWIthCallback::open()
{
    SineGenerator sineGenerator(TABLE_SIZE);


    // Set up the parameters required to open a (Callback)Stream:
    portaudio::System & portAudioSys = portaudio::System::instance();
    portaudio::DirectionSpecificStreamParameters outParams(portAudioSys.defaultOutputDevice(), 2, portaudio::FLOAT32, false, portAudioSys.defaultOutputDevice().defaultLowOutputLatency(), NULL);
    portaudio::StreamParameters params(portaudio::DirectionSpecificStreamParameters::null(), outParams, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff);

    //portaudio::MemFunCallbackStream<SineGenerator> stream(params, sineGenerator, &SineGenerator::generate);
    portaudio::MemFunCallbackStream<SineGenerator> stream;
    stream.open(params, sineGenerator, &SineGenerator::generate);

    stream.start();

    portAudioSys.sleep(NUM_SECONDS * 1000);

    stream.stop();

    stream.close();

    //for(auto device=portAudioSys.devicesBegin(); device != portAudioSys.devicesEnd(); ++device)
    //    qDebug()<<device->name();
    qDebug()<<portAudioSys.deviceCount();
    qDebug()<<portAudioSys.defaultOutputDevice().index();
    qDebug()<<openmpt::get_library_version();


    portAudioSys.sleep(NUM_SECONDS * 1000);
    return 0;

}

BeeperWIthCallback::BeeperWIthCallback() {

}

BeeperWIthCallback::~BeeperWIthCallback() {

}
