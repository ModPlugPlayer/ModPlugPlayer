#ifndef ENUMS_HPP
#define ENUMS_HPP
#include <QMetaType>

enum PlayerState {
    Playing,
    Paused,
    Stopped
};

enum SongState {
    Loaded,
    NotLoaded,
    Error
};

enum RepeatState{
    RepeatForewer = -1,
    DoNotRepeat = 0
};

enum SampleRate {
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
    Hz96000=96000,
    Hz192000=192000
};

enum BitRate {
    Bits8 = 8,
    Bits16 = 16,
    Bits24 = 24,
    Bits32 = 32
};

enum InterpolationFilter {
    Internal = 0, //default
    NoInterpolation = 1, //(zero order hold)
    LinearInterpolation = 2,
    CubicInterpolation = 4,
    WindowedSincWith8Taps = 8
};

Q_DECLARE_METATYPE(InterpolationFilter);

#endif // ENUMS_HPP
