#include "LCDPanel.hpp"
#include "ui_LCDPanel.h"
#include <QFontDatabase>
#include <QFile>
#include <QDebug>

LCDPanel::LCDPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LCDPanel)
{
    ui->setupUi(this);
    QFontDatabase::addApplicationFont(":/Fonts/Seven Segment.ttf");
    //url(:/Fonts/Seven Segment.ttf);
        SevenSegment = new QFont("Seven Segment", 25, QFont::Light);
        QFile file(":/Fonts/Seven Segment.ttf");
         qDebug() << file.exists();
        ui->secondDigit1->setFont(*SevenSegment);
        ui->secondDigit2->setFont(*SevenSegment);
        ui->minuteDigit1->setFont(*SevenSegment);
        ui->minuteDigit2->setFont(*SevenSegment);
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

LCDPanel::~LCDPanel()
{
    delete SevenSegment;
    delete ui;
}
