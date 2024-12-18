#include "KissFFTImpl.hpp"

#include <QDebug>

template<> void KissFFTImpl<float>::initialize(size_t inputDataElementAmount) {
    this->inputDataElementAmount = inputDataElementAmount;
    outputDataElementAmount = inputDataElementAmount/2+1;
    cfg = kiss_fftr_alloc(inputDataElementAmount, 0, nullptr, nullptr); // 0: forward FFT

    fftInput = new float[inputDataElementAmount];
    fftOutput = new std::complex<float>[outputDataElementAmount];

    kiss_output = (kiss_fft_cpx*)malloc((outputDataElementAmount) * sizeof(kiss_fft_cpx));


}


template<> void KissFFTImpl<float>::execute() {
    kiss_fftr(cfg, fftInput, kiss_output);
    for (size_t i = 0; i < outputDataElementAmount; i++) {
        fftOutput[i] = std::complex<float>(kiss_output[i].r, kiss_output[i].i);
    }
}


