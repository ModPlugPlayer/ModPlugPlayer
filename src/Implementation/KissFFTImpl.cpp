#include "KissFFTImpl.hpp"
#include <Util/FFTUtil.hpp>

#include <QDebug>

using namespace ModPlugPlayer;

template<> void KissFFTImpl<float>::initialize(size_t inputDataElementAmount) {
    this->inputDataElementAmount = inputDataElementAmount;
    outputDataElementAmount = FFTUtil::getOutputDataElementAmount(inputDataElementAmount);
    kissConfig = kiss_fftr_alloc(inputDataElementAmount, 0, nullptr, nullptr); // 0: forward FFT

    fftInput = new float[inputDataElementAmount];
    fftOutput = new std::complex<float>[outputDataElementAmount];

    kissOutput = (kiss_fft_cpx*)malloc((outputDataElementAmount) * sizeof(kiss_fft_cpx));
}


template<> void KissFFTImpl<float>::execute() {
    kiss_fftr(kissConfig, fftInput, kissOutput);
    for (size_t i = 0; i < outputDataElementAmount; i++) {
        fftOutput[i] = std::complex<float>(kissOutput[i].r, kissOutput[i].i);
    }
}
