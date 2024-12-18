#pragma once

#include <Interfaces/FFT.hpp>
#include <fftw3.h>
template<class T>
class FFTWImpl : public ModPlugPlayer::Interfaces::FFT<T>
{
public:
    FFTWImpl(size_t bufferSize, size_t fftPrecision);
    void initialize();
    void execute();
    void close();
private:
    void *fftPlan = nullptr;
    size_t bufferSize = 0;
    size_t fftPrecision = 0;
    unsigned int fftPlanType = FFTW_MEASURE;
};

template<class T> FFTWImpl<T>::FFTWImpl(size_t bufferSize, size_t fftPrecision) {
    this->bufferSize = bufferSize;
    this->fftPrecision = fftPrecision;
}
