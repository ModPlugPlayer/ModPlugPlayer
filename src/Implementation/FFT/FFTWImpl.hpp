#pragma once

#include <Interfaces/FFT.hpp>
#include <fftw3.h>
template<class T>
class FFTWImpl : public ModPlugPlayer::Interfaces::FFT<T>
{
public:
    void initialize(size_t inputDataElementAmount) override;
    void execute() override;
    void close() override;
private:
    void *fftPlan = nullptr;
    unsigned int fftPlanType = FFTW_MEASURE;
};
