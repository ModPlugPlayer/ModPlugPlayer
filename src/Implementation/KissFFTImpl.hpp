#pragma once

#include <Interfaces/FFT.hpp>
#include "../KissFFT/kiss_fft.h"
template<class T>
class KissFFTImpl : public ModPlugPlayer::Interfaces::FFT<T>
{
public:
    void initialize(size_t inputDataElementAmount);
    void execute();
    void close();
private:
    kiss_fft_cfg cfg = nullptr;
    kiss_fft_cpx* kiss_input;
    kiss_fft_cpx* kiss_output;
    size_t inputDataElementAmount = 0;
};

template<class T> void KissFFTImpl<T>::close() {
    free(cfg);
    free(this->fftInput);
    free(this->fftOutput);
}
