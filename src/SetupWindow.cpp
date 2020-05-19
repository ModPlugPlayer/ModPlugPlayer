#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"

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

void SetupWindow::on_stereoSeparationSlider_sliderMoved(int position)
{
	switch (position) {
		case 0:
			ui->labelStereoSeparation->setText("25%");
			break;
		case 1:
			ui->labelStereoSeparation->setText("50%");
			break;
		case 2:
			ui->labelStereoSeparation->setText("100%");
			break;
		case 3:
			ui->labelStereoSeparation->setText("200%");
			break;
		case 4:
			ui->labelStereoSeparation->setText("400%");
			break;
	}
}
