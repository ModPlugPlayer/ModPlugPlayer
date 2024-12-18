#include "KissFFTImpl.hpp"

#include <QDebug>

template<> void KissFFTImpl<float>::initialize(size_t inputDataElementAmount) {
    this->inputDataElementAmount = inputDataElementAmount;
    kiss_fft_cfg cfg = kiss_fft_alloc(inputDataElementAmount, 0, nullptr, nullptr); // 0: forward FFT

    kiss_input = (kiss_fft_cpx*)malloc(inputDataElementAmount * sizeof(kiss_fft_cpx));
    kiss_output = (kiss_fft_cpx*)malloc(inputDataElementAmount * sizeof(kiss_fft_cpx));


}

template<> void KissFFTImpl<float>::execute() {
    for (size_t i = 0; inputDataElementAmount; i++) {
        kiss_input[i].r = fftInput[i]; // Gerçek kısmı
        kiss_input[i].i = 0.0f;    // Sanal kısmı sıfır
    }

    kiss_fft(cfg, kiss_input, kiss_output);
}


