/*
TitleBar class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "TitleBar.hpp"
#include "ui_TitleBar.h"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
}
void TitleBar::setActiveColor(const RGB &color)
{
	this->activeColor = color;
	setStyleSheetColor(color);
}

RGB TitleBar::getActiveColor()
{
	return activeColor;
}

void TitleBar::setInactiveColor(const RGB &color)
{
	this->inactiveColor = color;
	//setStyleSheetColor(color);
}

RGB TitleBar::getInactiveColor()
{
    return inactiveColor;
}

void TitleBar::setTitle(QString title)
{
    ui->label->setText(title);
}

TitleBar::~TitleBar()
{
	delete ui;
}

void TitleBar::setStyleSheetColor(RGB color)
{
	ui->label->setStyleSheet(QString("QLabel{color:\"%1\"}").arg(color.hex().c_str()));
}
