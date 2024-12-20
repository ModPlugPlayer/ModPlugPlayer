/*
FFTUtil namespace declarations of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <cstddef>
//#include <complex>

namespace  ModPlugPlayer::FFTUtil {
    size_t getOutputDataElementAmount(size_t inputDataElementAmount);
    template <class T> T getFrequencySpacingOld(size_t sampleRate, size_t fftPrecision);
    template <class T> T getFrequencySpacing(size_t sampleRate, size_t inputDataElementAmount);
}

template<class T> T ModPlugPlayer::FFTUtil::getFrequencySpacingOld(size_t sampleRate, size_t fftPrecision) {
    return T(sampleRate)/(fftPrecision-1);
}

template<class T> T ModPlugPlayer::FFTUtil::getFrequencySpacing(size_t sampleRate, size_t inputDataElementAmount) {
    return T(sampleRate)/inputDataElementAmount;
}
