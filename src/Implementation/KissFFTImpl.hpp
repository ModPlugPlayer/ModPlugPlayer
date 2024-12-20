#pragma once

#include <Interfaces/FFT.hpp>
#include "../Components/KissFFT/kiss_fftr.h"
template<class T>
class KissFFTImpl : public ModPlugPlayer::Interfaces::FFT<T>
{
public:
    void initialize(size_t inputDataElementAmount);
    void execute();
    void close();
private:
    kiss_fftr_cfg cfg = nullptr;
    kiss_fft_cpx* kiss_output;
    size_t inputDataElementAmount = 0;
    size_t outputDataElementAmount = 0;
};

template<class T> void KissFFTImpl<T>::close() {
    free(cfg);
    free(this->fftInput);
    free(this->fftOutput);
}
