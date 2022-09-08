/*
OptionButtons class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef OPTIONBUTTONS_HPP
#define OPTIONBUTTONS_HPP

#include <QWidget>
#include <RGB.hpp>
#include "LEDColorProperties.hpp"
#include "TextColorProperties.hpp"
#include "OptionButton.hpp"
#include <vector>

namespace Ui {
class OptionButtons;
}

class OptionButtons : public QWidget, public LEDColorProperties, TextColorProperties
{
    Q_OBJECT

public:
    explicit OptionButtons(QWidget *parent = nullptr);
    void setActiveButtonLightColor(const RGB &color);
    void setInactiveButtonLightColor(const RGB &color);
    void setBackgroundColor(const RGB &color);
    void setTextColor(const RGB &color);
    ~OptionButtons();
    void togglePlayListEditorButton(bool turnOn);
signals:
    void info();
    void repeat();
    void saveAs();
    void del();
    void details();
    void shuffle();
    void saveList();
    void clear();
    void about();
    void playlist(bool turnedOn);
    void saveWav();
    void remove();
private:
    Ui::OptionButtons *ui;
protected:
    void refreshStyleSheet() override;
    std::vector<OptionButton *> buttons;
};

#endif // OPTIONBUTTONS_HPP
