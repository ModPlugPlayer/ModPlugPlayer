/*
OptionButton class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "OptionButton.hpp"
#include <QPainter>
#include "SVGIconEngine.hpp"
#include "ResourceUtil.hpp"

OptionButton::OptionButton(QWidget* parent):QPushButton(parent)
{
    setActiveButtonLightColor(RGB(std::string("#00FF00")));
    setInactiveButtonLightColor(RGB(std::string("#00FFFF")));
    buttonLightSize = 8;
    buttonLightPaddingSize = 3;
    buttonLightBorderSize = 0.4;
    iconButtonLight = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/toprightcorner.svg"), "#00ff00");
}

void OptionButton::setActiveButtonLightColor(const RGB & color)
{
    LedButton::setActiveButtonLightColor(color);
    active = QColor(color.red, color.green, color.blue);
    //refreshStyleSheet();
    //iconButtonLight->setActiveColor(color);

}

void OptionButton::setInactiveButtonLightColor(const RGB & color)
{
    LedButton::setInactiveButtonLightColor(color);
    inactive = QColor(color.red, color.green, color.blue);
    //refreshStyleSheet();
    //iconButtonLight->setInactiveColor(color);
}

void OptionButton::setBackgroundColor(const RGB & color)
{
    LedButton::setBackgroundColor(color);
    refreshStyleSheet();
}

void OptionButton::setTextColor(const RGB & color)
{
    LedButton::setTextColor(color);
    refreshStyleSheet();
}

void OptionButton::paintEvent(QPaintEvent * event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::VerticalSubpixelPositioning);
    painter.setPen(QPen(Qt::black, buttonLightBorderSize));
    int shiftingAmount;
    if(QPushButton::isDown()) {
        shiftingAmount = 1;
        painter.setBrush(active);
    }
    else {
        shiftingAmount = 0;
        painter.setBrush(inactive);
    }
    QPolygon pp;
    pp << QPoint(width() - buttonLightSize - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) << QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) <<QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightSize + buttonLightPaddingSize + shiftingAmount);
    painter.drawPolygon(pp, Qt::OddEvenFill);
    painter.restore();
}

void OptionButton::refreshStyleSheet() {
    QString style = QString("background-color:%1; color:%2;").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    setStyleSheet(style);
}
