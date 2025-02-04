/*
TitleBar class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "TitleBar.hpp"
#include "ui_TitleBar.h"
#include <Util/WindowUtil.hpp>

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar) {
    ui->setupUi(this);

    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::minimizeButtonClicked, this, &TitleBar::minimizeButtonClicked);
    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::miniPlayerButtonClicked, this, &TitleBar::miniPlayerButtonClicked);
    QObject::connect(ui->systemCaptionButtons, &SystemCaptionButtons::closeButtonClicked, this, &TitleBar::closeButtonClicked);

    #ifdef Q_OS_MACOS
    ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        ui->icon->hide();
        ui->systemCaptionButtons->hide();
        titleFontSize = 13;
        ui->bottomSpacer->changeSize(1, 4, QSizePolicy::Fixed, QSizePolicy::Fixed);
        //Makes this component seem under the transparent titlebar correctly
        setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
    #else
        ui->label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->leftSpacer->changeSize(2,0);
        #ifdef Q_OS_WINDOWS
            titleFontSize = 11;
        #else
            titleFontSize = 10;
        #endif
    #endif
        ui->label->setMinimumSize(16,16);
}

void TitleBar::setActiveColor(const RGB &color) {
	this->activeColor = color;
	setStyleSheetColor(color);
}

RGB TitleBar::getActiveColor() {
	return activeColor;
}

void TitleBar::setInactiveColor(const RGB &color) {
	this->inactiveColor = color;
	//setStyleSheetColor(color);
}

RGB TitleBar::getInactiveColor() {
    return inactiveColor;
}

void TitleBar::setTitleByFilePath(std::filesystem::path filePath) {
    this->filePath = filePath;
    updateTitleBar();
}

QFont TitleBar::labelFont() {
    return ui->label->font();
}

size_t TitleBar::labelWidth() {
    return ui->label->width();
}

TitleBar::~TitleBar() {
	delete ui;
}

void TitleBar::setStyleSheetColor(RGB color) {
    ui->label->setStyleSheet(QString("font-size: %1px;QLabel{color:\"%2\"}").arg(titleFontSize).arg(color.hex().c_str()));
}

void TitleBar::updateTitleBar() {
    QString titleBarText = QString("ModPlug Player - ") + QString::fromStdString(filePath.filename());
    QString stem = QString::fromStdString(filePath.stem());
    QString extension = QString::fromStdString(filePath.extension());
    if(extension.size() <= 4)
        titleBarText = WindowUtil::shortenTextToWidth(labelFont(), labelWidth(), QString("ModPlug Player - ") + stem, extension);
    else
        titleBarText = WindowUtil::shortenTextToWidth(labelFont(), labelWidth(), titleBarText);

    ui->label->setText(titleBarText);
}

void TitleBar::resizeEvent(QResizeEvent *event) {
    updateTitleBar();
}
