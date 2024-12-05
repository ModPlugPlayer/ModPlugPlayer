/*
OptionButtons class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "OptionButtons.hpp"
#include "ui_OptionButtons.h"

OptionButtons::OptionButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionButtons)
{
    ui->setupUi(this);
    buttons.push_back(ui->infoButton);
    buttons.push_back(ui->repeatButton);
    buttons.push_back(ui->filterButton);
    buttons.push_back(ui->amigaButton);
    buttons.push_back(ui->detailsButton);
    buttons.push_back(ui->shuffleButton);
    buttons.push_back(ui->dspButton);
    buttons.push_back(ui->eqButton);
    buttons.push_back(ui->aboutButton);
    buttons.push_back(ui->playlistButton);
    buttons.push_back(ui->dspOpButton);
    buttons.push_back(ui->diskOpButton);
    connect(ui->infoButton, SIGNAL(clicked()), this, SIGNAL(info()));
    connect(ui->repeatButton, SIGNAL(clicked()), this, SIGNAL(repeat()));
    connect(ui->filterButton, SIGNAL(clicked()), this, SIGNAL(filter()));
    connect(ui->amigaButton, SIGNAL(clicked()), this, SIGNAL(amiga()));
    connect(ui->detailsButton, SIGNAL(clicked()), this, SIGNAL(details()));
    connect(ui->shuffleButton, SIGNAL(clicked()), this, SIGNAL(shuffle()));
    connect(ui->dspButton, SIGNAL(clicked()), this, SIGNAL(dsp()));
    connect(ui->eqButton, SIGNAL(clicked()), this, SIGNAL(eq()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SIGNAL(about()));
    connect(ui->playlistButton, &OptionButton::toggled, this, &OptionButtons::playlist);
    connect(ui->dspOpButton, SIGNAL(clicked()), this, SIGNAL(dsp()));
    connect(ui->diskOpButton, SIGNAL(clicked()), this, SIGNAL(diskOp()));
    ui->playlistButton->setStateful(true);
}

void OptionButtons::setActiveButtonLightColor(const RGB & color)
{
    for(OptionButton *button : buttons) {
        button->setActiveLEDColor(color);
    }
}

void OptionButtons::setInactiveButtonLightColor(const RGB & color)
{
    for(OptionButton *button : buttons) {
        button->setInactiveLEDColor(color);
    }
}

void OptionButtons::setBackgroundColor(const RGB & color)
{
    for(OptionButton *button : buttons) {
        button->setBackgroundColor(color);
    }
}

void OptionButtons::setTextColor(const RGB & color)
{
    for(OptionButton *button : buttons) {
        button->setTextColor(color);
    }
}

OptionButtons::~OptionButtons()
{
    delete ui;
}

void OptionButtons::togglePlayListEditorButton(bool turnOn)
{
    ui->playlistButton->setTurnedOn(turnOn);
}

void OptionButtons::refreshStyleSheet()
{

}
