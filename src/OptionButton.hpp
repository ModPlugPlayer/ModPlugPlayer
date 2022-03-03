/*
OptionButton class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef OPTIONBUTTON_HPP
#define OPTIONBUTTON_HPP

#include <QPushButton>
#include "SVGIcon.hpp"
#include <RGB.hpp>
#include "LedButton.hpp"

class OptionButton : public QPushButton, public LedButton
{
		Q_OBJECT
	public:
        OptionButton(QWidget* parent = 0);
        void setActiveButtonLightColor(const RGB &color) override;
        void setInactiveButtonLightColor(const RGB &color) override;
        void setBackgroundColor(const RGB &color) override;
        void setTextColor(const RGB &color) override;
        bool isStateful() const;
        void setStateful(const bool &stateful);
        bool isTurnedOn() const;
        void setTurnedOn(const bool &turnedOn);
        void toggle();
    private:
        RGB activeButtonLightColor, inactiveButtonLightColor;
        QColor active, inactive;
        int buttonLightSize;
        int buttonLightPaddingSize;
        SVGIcon *iconButtonLight;
        qreal buttonLightBorderSize;
        bool stateful = false;
        bool turnedOn = false;
    private slots:
        /// paint the widget
        virtual void paintEvent(QPaintEvent* event);
        void onClick();
    protected:
        void refreshStyleSheet() override;
    signals:
        void toggled(bool turnedOn);
};

#endif // OPTIONBUTTON_HPP
