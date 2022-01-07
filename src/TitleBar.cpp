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

    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::minimizeButtonClicked, this, &TitleBar::minimizeButtonClicked);
    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::miniPlayerButtonClicked, this, &TitleBar::miniPlayerButtonClicked);
    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::closeButtonClicked, this, &TitleBar::closeButtonClicked);

    #ifdef Q_OS_MACOS
        ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->icon->hide();
        ui->systemCaptionButtons->hide();
        titleFontSize = 13;
    #else
        ui->label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->leftSpacer->changeSize(2,0);
        titleFontSize = 11;
    #endif
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
    ui->label->setStyleSheet(QString("font-size: %1px;QLabel{color:\"%2\"}").arg(titleFontSize).arg(color.hex().c_str()));
}
