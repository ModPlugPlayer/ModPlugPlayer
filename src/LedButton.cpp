/*
LedButton class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "LedButton.hpp"

void LedButton::setActiveButtonLightColor(const RGB & color) {
    activeButtonLightColor = color;
}

void LedButton::setInactiveButtonLightColor(const RGB & color) {
    inactiveButtonLightColor = color;
}

RGB LedButton::getActiveButtonLightColor() const {
    return activeButtonLightColor;
}

RGB LedButton::getInactiveButtonLightColor() const {
    return inactiveButtonLightColor;
}
