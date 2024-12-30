#pragma once

#include <Interfaces/FFT.hpp>
#include "../Components/KissFFT/kiss_fftr.h"
template<class T>
class KissFFTImpl : public ModPlugPlayer::Interfaces::FFT<T>
{
public:
    void initialize(size_t inputDataElementAmount) override;
    void execute() override;
    void close() override;
private:
    kiss_fftr_cfg kissConfig = nullptr;
    kiss_fft_cpx* kissOutput;
    size_t inputDataElementAmount = 0;
    size_t outputDataElementAmount = 0;
};

template<class T> void KissFFTImpl<T>::close() {
    free(kissConfig);
    free(kissOutput);
    free(this->fftInput);
    free(this->fftOutput);
}
