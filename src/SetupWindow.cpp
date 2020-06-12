#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"
#include <cmath>
#include <QDebug>
#include <QColorDialog>
#include <portaudiocpp/PortAudioCpp.hxx>

SetupWindow::SetupWindow(MppParameters *parameters, PlayerWindow *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow)
{
	this->playerWindow = parent;
	this->parameters = parameters;
	initAudioIcons();

	ui->setupUi(this);
	load();
	initAudioInterfaceList();
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

void SetupWindow::initAudioIcons()
{
	iconCoreAudio.addPixmap(QPixmap(":/Graphics/Raster/CoreAudio.png"));
	iconWdmAudio.addPixmap(QPixmap(":/Graphics/Raster/WDM.png"));
	iconDirectXAudio.addPixmap(QPixmap(":/Graphics/Raster/DirectX.png"));
	iconAlsaAudio.addPixmap(QPixmap(":/Graphics/Raster/alsa.png"));
	iconOssAudio.addPixmap(QPixmap(":/Graphics/Raster/oss.png"));
	iconAsioAudio.addPixmap(QPixmap(":/Graphics/Raster/ASIO.png"));
	iconJackAudio.addPixmap(QPixmap(":/Graphics/Raster/JackAudio.png"));
}

void SetupWindow::initAudioInterfaceList()
{
	portaudio::System &sys = portaudio::System::instance();
	int numDevices = sys.deviceCount();

	try {
		for (portaudio::System::DeviceIterator i = sys.devicesBegin(); i != sys.devicesEnd(); ++i){
			std::cout << "--------------------------------------- device #" << (*i).index() << std::endl;

			// Mark global and API specific default devices:
			bool defaultDisplayed = false;

			if ((*i).isSystemDefaultInputDevice())
			{
				std::cout << "[ Default Input";
				defaultDisplayed = true;
			}
			else if ((*i).isHostApiDefaultInputDevice())
			{
				std::cout << "[ Default " << (*i).hostApi().name() << " Input";
				defaultDisplayed = true;
			}

			if ((*i).isSystemDefaultOutputDevice())
			{
				std::cout << (defaultDisplayed ? "," : "[");
				std::cout << " Default Output";
				defaultDisplayed = true;
			}
			else if ((*i).isHostApiDefaultOutputDevice())
			{
				std::cout << (defaultDisplayed ? "," : "[");
				std::cout << " Default " << (*i).hostApi().name() << " Output";
				defaultDisplayed = true;
			}

			if (defaultDisplayed)
				std::cout << " ]" << std::endl;
			if(!(*i).isInputOnlyDevice()){
				QString devStr = QString("%1 - %2").arg((*i).hostApi().name(), (*i).name());
				if((*i).isSystemDefaultOutputDevice() || (*i).isHostApiDefaultOutputDevice())
					devStr += " (Default)";
				ui->comboBoxSoundDevices->addItem(iconCoreAudio, devStr, (*i).index());
			}
			// Print device info:
			std::cout << "Name                        = " << (*i).name() << std::endl;
			std::cout << "Host API                    = " << (*i).hostApi().name() << std::endl;
			std::cout << "Max inputs = " << (*i).maxInputChannels() << ", Max outputs = " << (*i).maxOutputChannels() << std::endl;

			std::cout << "Default low input latency   = " << (*i).defaultLowInputLatency() << std::endl; // 8.3
			std::cout << "Default low output latency  = " << (*i).defaultLowOutputLatency() << std::endl; // 8.3
			std::cout << "Default high input latency  = " << (*i).defaultHighInputLatency() << std::endl; // 8.3
			std::cout << "Default high output latency = " << (*i).defaultHighOutputLatency() << std::endl; // 8.3

		}
	}
	catch (const portaudio::PaException &e)
	{
		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
	}
	catch (const portaudio::PaCppException &e)
	{
		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "A generic exception occured: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "An unknown exception occured." << std::endl;
	}
}

QIcon SetupWindow::getAudioIcon(std::string &hostApiName)
{
	if(hostApiName == "Core Audio")
		return iconCoreAudio;
	return QIcon();
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

void SetupWindow::on_pushButton_TitleBar_Active_clicked()
{

		QColor color = QColorDialog::getColor(Qt::yellow, this);
		int r, g, b, h, s, l;
		color.getRgb(&r, &g, &b);
		color.getHsl(&h, &s, &l);
		QString fgColor = l <= 127 ? "white" : "black";
		ui->pushButton_TitleBar_Active->setStyleSheet(QString("QPushButton {background-color:rgb(%1,%2,%3);\ncolor:%4}").arg(r).arg(g).arg(b).arg(fgColor));
}
