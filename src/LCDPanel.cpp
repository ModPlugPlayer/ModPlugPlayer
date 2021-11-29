
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

void LCDPanel::setSongDuration(size_t songDurationSeconds)
{
    QString text = QStringLiteral("%1:%2s").arg(songDurationSeconds/60, 2, 10, QLatin1Char('0')).arg(songDurationSeconds%60, 2, 10, QLatin1Char('0'));
    //text.sprintf("%02d:%02ds", songDurationSeconds/60, songDurationSeconds%60);
    ui->totalTime->setText(text);
}

void LCDPanel::setBackgroundColor(const RGB & color)
{
    PlayerButton::setBackgroundColor(color);
    refreshStyleSheet();
}

void LCDPanel::setTextColor(const RGB & color)
{
    PlayerButton::setTextColor(color);
    refreshStyleSheet();
}

LCDPanel::~LCDPanel()
{
    delete SevenSegment;
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
