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

TitleBar::~TitleBar()
{
	delete ui;
}

void TitleBar::setStyleSheetColor(RGB color)
{
	ui->label->setStyleSheet(QString("QLabel{color:\"%1\"}").arg(color.hex().c_str()));
}
