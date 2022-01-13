#pragma once
#include <portaudiocpp/PortAudioCpp.hxx>
#include <set>

namespace PortAudioUtil
{
    bool isFormatSupported(PaDeviceIndex deviceIndex, PaSampleFormat sampleFormat, int channelCount, int sampleRate);
    std::set<int> getSupportedSampleRates(PaDeviceIndex deviceIndex);
};

