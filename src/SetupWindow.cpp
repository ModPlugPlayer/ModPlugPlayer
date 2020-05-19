#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"
#include <cmath>

SetupWindow::SetupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow)
{
    ui->setupUi(this);
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->pages->setCurrentIndex(currentRow);
}

void SetupWindow::on_stereoSeparationSlider_valueChanged(int value)
{
	ui->labelStereoSeparation->setText(QString::number(std::pow(2, value)*25) + "%");
}
