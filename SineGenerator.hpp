#ifndef SINEGENERATOR_HPP
#define SINEGENERATOR_HPP
#include <libopenmpt/libopenmpt.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>


// Some constants:
const int NUM_SECONDS = 4;
const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 64;
const int TABLE_SIZE = 200;

// SineGenerator class:
class SineGenerator
{
public:
    SineGenerator(int tableSize);
    ~SineGenerator();

    int generate(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);

private:
    float *table_;
    int tableSize_;
    int leftPhase_;
    int rightPhase_;
};


#endif // SINEGENERATOR_HPP
