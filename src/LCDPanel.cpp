/*
LCDPanel class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "LCDPanel.hpp"
#include "ui_LCDPanel.h"
#include <QFile>
#include <QDebug>

LCDPanel::LCDPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LCDPanel)
{
    ui->setupUi(this);
    SevenSegment = new QFont("Seven Segment", 25, QFont::Light);
    InterFont = new QFont("Inter", 20, QFont::Normal);
    ui->secondDigit1->setFont(*SevenSegment);
    ui->secondDigit2->setFont(*SevenSegment);
    ui->minuteDigit1->setFont(*SevenSegment);
    ui->minuteDigit2->setFont(*SevenSegment);
    ui->songTitle->setFont(*InterFont);
    ui->colon->setFont(*SevenSegment);
}

void LCDPanel::updateTime(int seconds)
{
    ui->minuteDigit1->setText(QString::number(seconds/600));
    ui->minuteDigit2->setText(QString::number(seconds/60));
    ui->secondDigit1->setText(QString::number((seconds%60)/10));
    ui->secondDigit2->setText(QString::number((seconds%60)%10));

}

void LCDPanel::setSongTitle(QString songTitle)
{
    ui->songTitle->setText(songTitle);
}

void LCDPanel::setSongDuration(size_t songDurationSeconds)
{
    QString text = QStringLiteral("%1:%2s").arg(songDurationSeconds/60, 2, 10, QLatin1Char('0')).arg(songDurationSeconds%60, 2, 10, QLatin1Char('0'));
    //text.sprintf("%02d:%02ds", songDurationSeconds/60, songDurationSeconds%60);
    ui->totalTime->setText(text);
}

LCDPanel::~LCDPanel()
{
    delete SevenSegment;
    delete InterFont;
    delete ui;
}

void LCDPanel::refreshStyleSheet()
{
    //QString style = QString("background-color:%1; color:%2;").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    QString backgroundStyle = QString("background-color:%1;").arg(backgroundColor.hex().c_str());
    QString textStyle = QString("color:%1;").arg(textColor.hex().c_str());
    ui->background->setStyleSheet(backgroundStyle);
    ui->totalTime->setStyleSheet(backgroundStyle);
    setStyleSheet(textStyle);
}
