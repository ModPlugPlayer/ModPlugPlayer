#include "PortAudioUtil.hpp"

namespace PortAudioUtil {
    bool isFormatSupported(PaDeviceIndex deviceIndex, PaSampleFormat sampleFormat, int channelCount, int sampleRate) {
        PaStreamParameters outputParameters;
        outputParameters.device = deviceIndex;
        outputParameters.sampleFormat = sampleFormat;
        outputParameters.channelCount = channelCount;
        outputParameters.hostApiSpecificStreamInfo = nullptr;
        return (Pa_IsFormatSupported(NULL, &outputParameters, sampleRate) == paFormatIsSupported);
    }

    std::set<int> getSupportedSampleRates(PaDeviceIndex deviceIndex) {
        std::set<int> supportedSampleRates;
        for (int sampleRate : { 8000, 11000, 11025, 22000, 22050, 32000, 44000, 44100, 48000, 88200, 96000, 176400, 192000, 384000}) {
            bool found = false;
            for(PaSampleFormat sampleFormat : {paFloat32, paInt32, paInt24, paInt16, paInt8, paUInt8}) {
                for(int channelCount : {1, 2, 4, 6}) {
                    if (isFormatSupported(deviceIndex, sampleFormat, channelCount, sampleRate)) {
                        supportedSampleRates.insert(sampleRate);
                        found = true;
                        break;
                    }
                }
                if(found)
                    break;
            }
        }
        return supportedSampleRates;
    }

}
