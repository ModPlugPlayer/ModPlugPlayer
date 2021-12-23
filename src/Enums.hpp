/*
Enum definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ENUMS_HPP
#define ENUMS_HPP
#include <QMetaType>

enum class PlayerState {
    Playing,
    Paused,
    Stopped
};

enum class SongState {
    Loaded,
    NotLoaded,
    Error
};

enum class RepeatState:int {
    RepeatForewer = -1,
    DoNotRepeat = 0
};

enum class SampleRate:int {
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

enum class BitRate:int {
    Bits8 = 8,
    Bits16 = 16,
    Bits24 = 24,
    Bits32 = 32
};

enum class InterpolationFilter:int {
    Internal = 0, //default
    NoInterpolation = 1, //(zero order hold)
    LinearInterpolation = 2,
    CubicInterpolation = 4,
    WindowedSincWith8Taps = 8
};

enum class AmigaFilterType:int {
    Auto = 0,
    Amiga500 = 500,
    Amiga1200 = 1200,
    Unfiltered = -1,
    DisablePaulaEmulation = -2
};

Q_DECLARE_METATYPE(InterpolationFilter);

#endif // ENUMS_HPP
