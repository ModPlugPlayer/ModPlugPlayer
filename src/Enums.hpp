#ifndef ENUMS_HPP
#define ENUMS_HPP

enum PLAYERSTATE {
    PLAYING,
    PAUSED,
    STOPPED
};

enum SONGSTATE {
    LOADED,
    NOT_LOADED,
    ERROR
};

enum SAMPLERATE {
    Hz8000=8000,
    Hz9600=9600,
    Hz11025=11025,
    Hz12000=12000,
    Hz16000=16000,
    Hz22050=22050,
    Hz24000=24000,
    Hz32000=32000,
    Hz44100=44100,
    Hz48000=48000,
    Hz88200=88200,
    Hz96000=96000
};

enum BITRATE {
    BITS8 = 8,
    BITS16 = 16,
    BITS24 = 24,
    BITS32 = 32
};

enum INTERPOLATIONFILTER {
    INTERNAL = 0, //internal default
    NO_INTERPOLATION = 1, //no interpolation (zero order hold)
    LINEAR_INTERPOLATION = 2, //linear interpolation
    CUBIC_INTERPOLATION = 4, //cubic interpolation
    WINDOWED_SINC_WITH_8_TAPS = 8 //windowed sinc with 8 taps
};

#endif // ENUMS_HPP
