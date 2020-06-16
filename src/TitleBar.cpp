#include "TitleBar.hpp"
#include "ui_TitleBar.h"

TitleBar::TitleBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleBar)
{
    ui->setupUi(this);
}
void TitleBar::setColor(const RGB &color)
{
//	ui->label->set
}

RGB TitleBar::getColor()
{

}

TitleBar::~TitleBar()
{
    delete ui;
}
