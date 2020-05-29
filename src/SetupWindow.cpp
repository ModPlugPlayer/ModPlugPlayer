#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"
#include <cmath>
#include <QDebug>

SetupWindow::SetupWindow(MppParameters *parameters, PlayerWindow *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow)
{
	this->playerWindow = parent;
	this->parameters = parameters;
    ui->setupUi(this);
	load();
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
	ui->labelStereoSeparation->setText(QString::number(value) + "%");
}

void SetupWindow::on_checkBoxEnableSystemTray_toggled(bool checked)
{
	if(!checked) {
		ui->checkBoxMinimizeToSystemTray->setChecked(false);
		ui->checkBoxHideApplicationIcon->setChecked(false);
	}
	ui->groupBoxSystemTray->setEnabled(checked);
}

void SetupWindow::on_buttonBox_clicked(QAbstractButton *button) {
	if(button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
		qDebug()<<"ok";
		save();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Cancel)) {
		qDebug()<<"cancel";
		//load();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Apply)) {
		qDebug()<<"apply";
		save();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Reset)) {
		qDebug()<<"reset";
		load();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		qDebug()<<"restore defaults";
		restoreDefaults();
	}

}

void SetupWindow::load()
{
	qDebug()<<parameters->volume;
	qDebug()<<"load";
}

void SetupWindow::save()
{
	qDebug()<<"save";
}

void SetupWindow::restoreDefaults()
{
	qDebug()<<"restore defaults";
}

void SetupWindow::on_checkBoxSaveSettingsImmediately_toggled(bool checked)
{
	if(checked) {
		ui->buttonBox->hide();
	}
	else {
		ui->buttonBox->show();
	}
}

void SetupWindow::on_checkBoxHideTitleBar_toggled(bool checked)
{
	playerWindow->onHideTitleBarRequested(checked);
}
