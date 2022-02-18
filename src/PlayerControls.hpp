/*
PlayerControls interface of ModPlug Player
Copyright (C) 2022 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

class PlayerControls {
public:
    virtual void stop() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void changeVolume(int volume) = 0;
    virtual int getVolume() const = 0;
    virtual void setAlwaysOnTop(bool alwaysOnTop) = 0;
    virtual bool getAlwaysOnTop() const = 0;
    virtual void hideTitleBar(bool hide) = 0;
    virtual bool isTitleBarHidden() const = 0;
    virtual void setSnapToViewPort(bool snapToViewPort) = 0;
    virtual bool getSnapToViewPort() const = 0;
    virtual void setKeepStayingInViewPort(bool keepStayingInViewPort) = 0;
    virtual bool getKeepStayingInViewPort() const = 0;
};
