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
    buttons.push_back(ui->saveAsButton);
    buttons.push_back(ui->deleteButton);
    buttons.push_back(ui->detailsButton);
    buttons.push_back(ui->shuffleButton);
    buttons.push_back(ui->saveListButton);
    buttons.push_back(ui->clearButton);
    buttons.push_back(ui->aboutButton);
    buttons.push_back(ui->playlistButton);
    buttons.push_back(ui->saveWavButton);
    buttons.push_back(ui->removeButton);
    connect(ui->infoButton, SIGNAL(clicked()), this, SIGNAL(info()));
    connect(ui->repeatButton, SIGNAL(clicked()), this, SIGNAL(repeat()));
    connect(ui->saveAsButton, SIGNAL(clicked()), this, SIGNAL(saveAs()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SIGNAL(del()));
    connect(ui->detailsButton, SIGNAL(clicked()), this, SIGNAL(details()));
    connect(ui->shuffleButton, SIGNAL(clicked()), this, SIGNAL(shuffle()));
    connect(ui->saveListButton, SIGNAL(clicked()), this, SIGNAL(saveList()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SIGNAL(about()));
    connect(ui->playlistButton, &OptionButton::toggled, this, &OptionButtons::playlist);
    connect(ui->saveWavButton, SIGNAL(clicked()), this, SIGNAL(saveWav()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SIGNAL(remove()));
    ui->playlistButton->setStateful(true);
}

void OptionButtons::setActiveButtonLightColor(const RGB & color)
{
    for(LedButton *button : buttons) {
        button->setActiveButtonLightColor(color);
    }
}

void OptionButtons::setInactiveButtonLightColor(const RGB & color)
{
    for(LedButton *button : buttons) {
        button->setInactiveButtonLightColor(color);
    }
}

void OptionButtons::setBackgroundColor(const RGB & color)
{
    for(LedButton *button : buttons) {
        button->setBackgroundColor(color);
    }
}

void OptionButtons::setTextColor(const RGB & color)
{
    for(LedButton *button : buttons) {
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
