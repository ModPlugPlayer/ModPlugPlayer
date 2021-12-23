/*
PlayerButton class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <RGB.hpp>

class PlayerButton {
public:
    virtual void setBackgroundColor(const RGB &color);
    virtual void setTextColor(const RGB &color);
    RGB getBackgroundColor() const;
    RGB getTextColor() const;
    const QString & getStyleSheetTemplate() const;
    void setStyleSheetTemplate(const QString & styleSheetTemplate);

protected:
    RGB backgroundColor;
    RGB textColor;
    QString styleSheetTemplate;
    virtual void refreshStyleSheet() = 0;
};
