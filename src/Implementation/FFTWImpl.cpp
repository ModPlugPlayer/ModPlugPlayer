#include "FFTWImpl.hpp"
#include <QDebug>
#include "../Util/FFTUtil.hpp"

using namespace ModPlugPlayer;

template<> void FFTWImpl<float>::close();
template<> void FFTWImpl<double>::close();

template<> void FFTWImpl<float>::initialize(size_t inputDataElementAmount) {
    if(fftPlan != nullptr) {
        this->close();
    }
    size_t outputDataElementAmount = FFTUtil::getOutputDataElementAmount(inputDataElementAmount);
    fftInput = fftwf_alloc_real(inputDataElementAmount);
    fftOutput = reinterpret_cast<std::complex<float>*>(outputDataElementAmount);
    fftPlan = (void *) fftwf_plan_dft_r2c_1d(inputDataElementAmount, fftInput, reinterpret_cast<fftwf_complex*>(&fftOutput[0]), fftPlanType);
    // 2n-1 for example, for 12 fftplan would be 23

    if (!fftPlan){
        qDebug("plan not created");
    }
}

template<> void FFTWImpl<double>::initialize(size_t inputDataElementAmount) {
    if(fftPlan != nullptr) {
        close();
    }
    size_t outputDataElementAmount = FFTUtil::getOutputDataElementAmount(inputDataElementAmount);
    fftInput = fftw_alloc_real(inputDataElementAmount);
    fftOutput = reinterpret_cast<std::complex<double>*>(fftw_alloc_complex(inputDataElementAmount/2 + 1));
    fftPlan = (void *) fftw_plan_dft_r2c_1d(inputDataElementAmount, fftInput, reinterpret_cast<fftw_complex*>(&fftOutput[0]), fftPlanType);
    // 2n-1 for example, for 12 fftplan would be 23

    if (!fftPlan){
        qDebug("plan not created");
    }
}

template<> void FFTWImpl<float>::execute() {
    fftwf_execute((fftwf_plan) fftPlan);
}

template<> void FFTWImpl<double>::execute() {
    fftw_execute((fftw_plan) fftPlan);
}

template<> void FFTWImpl<float>::close() {
    fftwf_destroy_plan((fftwf_plan) fftPlan);
    fftwf_free(fftInput);
    fftwf_free(fftOutput);
}

template<> void FFTWImpl<double>::close() {
    fftw_destroy_plan((fftw_plan) fftPlan);
    fftw_free(fftInput);
    fftw_free(fftOutput);
    fftw_cleanup();
}
