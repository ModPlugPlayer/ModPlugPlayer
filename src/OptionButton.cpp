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
    setActiveLEDColor(RGB(std::string("#00FF00")));
    setInactiveLEDColor(RGB(std::string("#00FFFF")));
    buttonLightSize = 8;
    buttonLightPaddingSize = 3;
    buttonLightBorderSize = 0.4;
    iconButtonLight = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/toprightcorner.svg"), "#00ff00");
}

bool OptionButton::isStateful() const
{
    return stateful;
}

void OptionButton::setStateful(const bool &stateful)
{
    this->stateful = stateful;
    if(stateful)
        connect(this, &QPushButton::clicked, this, &OptionButton::onClick);
    else
        disconnect(this, &QPushButton::clicked, this, &OptionButton::onClick);
}

bool OptionButton::isTurnedOn() const
{
    return turnedOn;
}

void OptionButton::setTurnedOn(const bool &turnedOn)
{
    this->turnedOn = turnedOn;
    repaint();
}

void OptionButton::toggle()
{
    turnedOn = !turnedOn;
    repaint();
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
        painter.setBrush(getActiveLEDColor().qColor());
    }
    else {
        shiftingAmount = 0;
        if(stateful && turnedOn) {
            painter.setBrush(getActiveLEDColor().qColor());
        }
        else {
            painter.setBrush(getInactiveLEDColor().qColor());
        }
    }

    QPolygon pp;
    pp << QPoint(width() - buttonLightSize - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) << QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) <<QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightSize + buttonLightPaddingSize + shiftingAmount);
    painter.drawPolygon(pp, Qt::OddEvenFill);
    painter.restore();
}

void OptionButton::onClick() {
    if(stateful) {
        toggle();
        emit toggled(isTurnedOn());
    }
}

void OptionButton::refreshStyleSheet() {
    QString style = QString("background-color:%1; color:%2;").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    setStyleSheet(style);
}
