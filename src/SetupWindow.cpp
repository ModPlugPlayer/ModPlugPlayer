/*
SetupWindow class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"
#include <cmath>
#include <QDebug>
#include <QColorDialog>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <AndromedaDSP.hpp>
#include <QString>
#include "PortAudioUtil.hpp"
#include <Parameters.hpp>
#include <MessageCenter.hpp>
#include "SettingsCenter.hpp"

SetupWindow::SetupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow) {
	ui->setupUi(this);
    initAudioIcons();
    initBitDepthValues();
    initSamplingFrequencyValues();
    initChannelValues();
    initDitherValues();
    initAudioInterfaceList();
    ui->pages->setCurrentIndex(0);
    ui->treeMenu->expandAll();
    ui->treeMenu->hideColumn(1);
    ui->aheadTheSignalWarning->setHidden(true);
    ui->treeMenu->topLevelItem(0)->setSelected(true);
    #ifdef Q_OS_WINDOWS
        ui->checkBoxHideByCloseButton->setEnabled(false);
    #endif
    load();
    connectSignalsAndSlots();
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::onSetupWindowRequested()
{
    //parameters->save();
    //bool stateAlwaysOnTop = isAlwaysOnTop();
    //WindowUtil::setAlwaysOnTop(this, false);
    //SetupWindow setupWindow(parameters, this);
    show();
    //exec();
    //WindowUtil::setAlwaysOnTop(this, stateAlwaysOnTop);
}

void SetupWindow::onActiveTitleBarTextColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->activeTitlebarTextColor = RGB(ui->pushButton_TitleBar_Active->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onInactiveTitleBarTextColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->inactiveTitlebarTextColor = RGB(ui->pushButton_TitleBar_Inactive->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onActiveButtonLightColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
	parameters->activeButtonLightColor = RGB(ui->pushButton_ButtonLights_Active->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onInactiveButtonLightColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
	parameters->inactiveButtonLightColor = RGB(ui->pushButton_ButtonLights_Inactive->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onPlayerBodyTextColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
	parameters->playerBodyTextColor = RGB(ui->pushButton_PlayerBody_Text->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onPlayerBodyBackgroundColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
	parameters->playerBodyBackgroundColor = RGB(ui->pushButton_PlayerBody_Background->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onLcdDisplayForegroundColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->lcdDisplayForegroundColor = RGB(ui->pushButton_LCDDisplay_Foreground->getColor());
    if(immediateMode)
        parameters->save();
}

void SetupWindow::onLcdDisplayBackgroundColorChanged(){
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->lcdDisplayBackgroundColor = RGB(ui->pushButton_LCDDisplay_Background->getColor());
    if(immediateMode)
        parameters->save();
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
        emit MessageCenter::getInstance().events.settingsEvents.settingsChanged();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Cancel)) {
        qDebug()<<"cancel";
        load();
	}
	else if(button == ui->buttonBox->button(QDialogButtonBox::Apply)) {
		qDebug()<<"apply";
		save();
        emit MessageCenter::getInstance().events.settingsEvents.settingsChanged();
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

void SetupWindow::load() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    qDebug()<<parameters->volume;
    //parameters->load();
	ui->pushButton_TitleBar_Active->setColor(parameters->activeTitlebarTextColor);
	ui->pushButton_TitleBar_Inactive->setColor(parameters->inactiveTitlebarTextColor);
	ui->pushButton_ButtonLights_Active->setColor(parameters->activeButtonLightColor);
	ui->pushButton_ButtonLights_Inactive->setColor(parameters->inactiveButtonLightColor);
	ui->pushButton_PlayerBody_Text->setColor(parameters->playerBodyTextColor);
	ui->pushButton_PlayerBody_Background->setColor(parameters->playerBodyBackgroundColor);
	ui->pushButton_LCDDisplay_Foreground->setColor(parameters->lcdDisplayForegroundColor);
	ui->pushButton_LCDDisplay_Background->setColor(parameters->lcdDisplayBackgroundColor);
    ui->checkBoxHideByCloseButton->setChecked(parameters->hideByCloseButton);
    ui->checkBoxSaveSettingsImmediately->setChecked(parameters->saveSettingsImmediately);
    ui->checkBoxHideTitleBar->setChecked(parameters->hideTitleBar);
    ui->checkBoxEnableSystemTray->setChecked(parameters->enableSystemTray);
    ui->checkBoxSnapToViewPort->setChecked(parameters->snapToViewPort);
    ui->checkBoxKeepStayingInViewPort->setChecked(parameters->keepStayingInViewPort);
    ui->checkBoxAlwaysOnTop->setChecked(parameters->alwaysOnTop);
    ui->snappingThreshold->setValue(parameters->snappingThreshold);
    ui->spectrumAnalyzerLogarithmicScale->setChecked(parameters->spectrumAnalyzerScaleIsLogarithmic);

    ui->spectrumAnalyzerMaximumValue->setValue(parameters->spectrumAnalyzerMaximumValue);
    ui->spectrumAnalyzerType->setCurrentIndex(parameters->spectrumAnalyzerType == BarType::Discrete ? 0 : 1);
    ui->spectrumAnalyzerWindowFunction->setCurrentIndex(static_cast<int>((WindowFunction)parameters->spectrumAnalyzerWindowFunction));
    ui->spectrumAnalyzerBarWidthRatio->setValue(parameters->spectrumAnalyzerBarWidthRatio*100);
    ui->spectrumAnalyzerLedAmount->setValue(parameters->spectrumAnalyzerBarLedAmount);
    ui->spectrumAnalyzerLedHeightRatio->setValue(parameters->spectrumAnalyzerLedHeightRatio*100);
    ui->spectrumAnalyzerBarAmount->setValue(parameters->spectrumAnalyzerBarAmount);
    ui->spectrumAnalyzerDimmingRatio->setValue(parameters->spectrumAnalyzerDimmingRatio*100);
    ui->spectrumAnalyzerDimmedTransparencyRatio->setValue(parameters->spectrumAnalyzerDimmedTransparencyRatio*100);
    ui->spectrumAnalyzerColorRampEditor->setColorRamp(parameters->spectrumAnalyzerGradient);

    ui->vuMeterMaximumValue->setValue(parameters->vuMeterMaximumValue);
    ui->vuMeterMinimumValue->setValue(parameters->vuMeterMinimumValue);
    ui->vuMeterType->setCurrentIndex(parameters->vuMeterType == BarType::Discrete ? 0 : 1);
    ui->vuMeterLedAmount->setValue(parameters->vuMeterLedAmount);
    ui->vuMeterLedHeightRatio->setValue(parameters->vuMeterLedHeightRatio*100);
    ui->vuMeterDimmingRatio->setValue(parameters->vuMeterDimmingRatio*100);
    ui->vuMeterDimmedTransparencyRatio->setValue(parameters->vuMeterDimmedTransparencyRatio*100);
    ui->vuMeterColorRampEditor->setColorRamp(parameters->vuMeterGradient);

    selectAudioDevice(parameters->audioDeviceIndex);
    selectSampleRate(parameters->samplingFrequency);
    selectBitDepth(parameters->bitDepth);
    selectChannelMode(parameters->channelMode);

    immediateMode = parameters->saveSettingsImmediately;
}

void SetupWindow::save() {
    qDebug()<<"save";
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    int r,g,b;
	ui->pushButton_TitleBar_Active->getColor().getRgb(&r, &g, &b);
	parameters->activeTitlebarTextColor = RGB(r,g,b);
    parameters->hideByCloseButton = ui->checkBoxHideByCloseButton->isChecked();
    parameters->hideTitleBar = ui->checkBoxHideTitleBar->isChecked();
    parameters->enableSystemTray = ui->checkBoxEnableSystemTray->isChecked();
    parameters->saveSettingsImmediately = ui->checkBoxSaveSettingsImmediately->isChecked();
    parameters->snapToViewPort = ui->checkBoxSnapToViewPort->isChecked();
    parameters->keepStayingInViewPort = ui->checkBoxKeepStayingInViewPort->isChecked();
    parameters->alwaysOnTop = ui->checkBoxAlwaysOnTop->isChecked();
    parameters->spectrumAnalyzerWindowFunction = static_cast<WindowFunction>(ui->spectrumAnalyzerWindowFunction->currentIndex());
    parameters->spectrumAnalyzerType = ui->spectrumAnalyzerType->currentIndex() == 0 ? BarType::Discrete : BarType::Continuous;
    parameters->spectrumAnalyzerBarAmount = ui->spectrumAnalyzerBarAmount->value();
    parameters->spectrumAnalyzerBarLedAmount = ui->spectrumAnalyzerLedAmount->value();
    parameters->spectrumAnalyzerBarWidthRatio = double(ui->spectrumAnalyzerBarWidthRatio->value())/100;
    parameters->spectrumAnalyzerLedHeightRatio = double(ui->spectrumAnalyzerLedHeightRatio->value())/100;
    parameters->spectrumAnalyzerGradient = ui->spectrumAnalyzerColorRampEditor->getColorRamp();

    parameters->save();
}

void SetupWindow::restoreDefaults()
{
	qDebug()<<"restore defaults";
}

void SetupWindow::connectSignalsAndSlots() {
    connect(ui->pushButton_TitleBar_Active, SIGNAL(colorChanged()), this, SLOT(onActiveTitleBarTextColorChanged()));
    connect(ui->pushButton_TitleBar_Inactive, SIGNAL(colorChanged()), this, SLOT(onInactiveTitleBarTextColorChanged()));
    connect(ui->pushButton_ButtonLights_Active, SIGNAL(colorChanged()), this, SLOT(onActiveButtonLightColorChanged()));
    connect(ui->pushButton_ButtonLights_Inactive, SIGNAL(colorChanged()), this, SLOT(onInactiveButtonLightColorChanged()));
    connect(ui->pushButton_PlayerBody_Text, SIGNAL(colorChanged()), this, SLOT(onPlayerBodyTextColorChanged()));
    connect(ui->pushButton_PlayerBody_Background, SIGNAL(colorChanged()), this, SLOT(onPlayerBodyBackgroundColorChanged()));
    connect(ui->pushButton_LCDDisplay_Foreground, SIGNAL(colorChanged()), this, SLOT(onLcdDisplayForegroundColorChanged()));
    connect(ui->pushButton_LCDDisplay_Background, SIGNAL(colorChanged()), this, SLOT(onLcdDisplayBackgroundColorChanged()));
    connect(ui->comboBoxSoundDevices, &QComboBox::activated, this, &SetupWindow::on_comboBoxSoundDevices_currentIndexActivated);
    connect(ui->spectrumAnalyzerColorRampEditor, &ColorRampEditor::colorRampChanged, this, &SetupWindow::onSpectrumAnalyzerColorRampChanged);
    connect(ui->vuMeterColorRampEditor, &ColorRampEditor::colorRampChanged, this, &SetupWindow::onVuMeterColorRampChanged);
    connect(&MessageCenter::getInstance().requests.windowStandardRequests.settingsWindowRequests, qOverload<>(&MessageCenterRequests::WindowStandardRequests::windowOpenRequested), this, &SetupWindow::onSetupWindowRequested);

    connect(ui->bitDepths, &QComboBox::currentIndexChanged, this, &SetupWindow::onBitDepthsComboBoxCurrentIndexChanged);
    connect(ui->samplingFrequencies, &QComboBox::currentIndexChanged, this, &SetupWindow::onSamplingFrequenciesComboBoxCurrentIndexChanged);
    connect(ui->channels, &QComboBox::currentIndexChanged, this, &SetupWindow::onChannelsComboBoxCurrentIndexChanged);
    connect(ui->dithers, &QComboBox::currentIndexChanged, this, &SetupWindow::onDithersComboBoxCurrentIndexChanged);
    connect(&MessageCenter::getInstance().requests.settingsRequests, &MessageCenterRequests::SettingsRequests::settingsSavingImmediatelyModeChangeRequested, this, &SetupWindow::onSettingsSavingImmediatelyModeChangeRequested);
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

void SetupWindow::initBitDepthValues() {
    ui->bitDepths->setItemData(0, QVariant(8));
    ui->bitDepths->setItemData(1, QVariant(16));
    ui->bitDepths->setItemData(2, QVariant(24));
}

void SetupWindow::initSamplingFrequencyValues() {
    ui->samplingFrequencies->setItemData(0, QVariant(11000));
    ui->samplingFrequencies->setItemData(1, QVariant(22000));
    ui->samplingFrequencies->setItemData(2, QVariant(24000));
    ui->samplingFrequencies->setItemData(3, QVariant(44100));
    ui->samplingFrequencies->setItemData(4, QVariant(48000));
    ui->samplingFrequencies->setItemData(5, QVariant(96000));
    ui->samplingFrequencies->setItemData(6, QVariant(192000));
}

void SetupWindow::initChannelValues()
{
    ui->channels->setItemData(0, QVariant(1));
    ui->channels->setItemData(1, QVariant(2));
    ui->channels->setItemData(2, QVariant(4));
    ui->channels->setItemData(3, QVariant(6));
    ui->channels->setItemData(4, QVariant());
    ui->channels->setItemData(5, QVariant());
}

void SetupWindow::initDitherValues() {
    ui->dithers->setItemData(0, QVariant(1));
    ui->dithers->setItemData(0, QVariant(0));
}

void SetupWindow::initAudioInterfaceList() {
    portaudio::System &sys = portaudio::System::instance();

	try {
        addDeviceToDeviceList(sys.defaultOutputDevice());

        for (portaudio::System::DeviceIterator device = sys.devicesBegin(); device != sys.devicesEnd(); ++device){
            if(device->isSystemDefaultOutputDevice() || device->isInputOnlyDevice())
                continue;
            addDeviceToDeviceList(*device);
        }
	}
    catch(const portaudio::PaException &e) {
		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
	}
    catch(const portaudio::PaCppException &e) {
		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
	}
    catch(const std::exception &e) {
		std::cout << "A generic exception occured: " << e.what() << std::endl;
	}
    catch(...) {
        std::cout << "An unknown exception occured." << std::endl;
    }
}

void SetupWindow::addDeviceToDeviceList(portaudio::Device & device) {
    std::cout << "--------------------------------------- device #" << device.index() << std::endl;

    // Mark global and API specific default devices:
    bool defaultDisplayed = false;
    QIcon * icon = &emptyIcon;

    if(device.isSystemDefaultInputDevice()) {
        std::cout << "[ Default Input";
        defaultDisplayed = true;
    }
    else if(device.isHostApiDefaultInputDevice()) {
        std::cout << "[ Default " << device.hostApi().name() << " Input";
        defaultDisplayed = true;
    }

    if(device.isSystemDefaultOutputDevice()) {
        std::cout << (defaultDisplayed ? "," : "[");
        std::cout << " Default Output";
        defaultDisplayed = true;
    }
    else if (device.isHostApiDefaultOutputDevice()) {
        std::cout << (defaultDisplayed ? "," : "[");
        std::cout << " Default " << device.hostApi().name() << " Output";
        defaultDisplayed = true;
    }

    if(defaultDisplayed)
        std::cout << " ]" << std::endl;
    if(!device.isInputOnlyDevice()) {
        QString devStr = QString("%1 - %2").arg(device.hostApi().name(), device.name());
        if(device.isSystemDefaultOutputDevice() || device.isHostApiDefaultOutputDevice())
            devStr += " (Default)";
        switch(device.hostApi().typeId()) {
        case PaHostApiTypeId::paALSA:
            icon = &iconAlsaAudio;
            break;
        case PaHostApiTypeId::paOSS:
            icon = &iconOssAudio;
            break;
        case PaHostApiTypeId::paCoreAudio:
            icon = &iconCoreAudio;
            break;
        case PaHostApiTypeId::paDirectSound:
            icon = &iconDirectXAudio;
            break;
        case PaHostApiTypeId::paWDMKS:
        case PaHostApiTypeId::paWASAPI:
        case PaHostApiTypeId::paMME:
            icon = &iconWdmAudio;
            break;
        case PaHostApiTypeId::paJACK:
            icon = &iconAsioAudio;
            break;
        default:
            break;
        }

        ui->comboBoxSoundDevices->addItem(*icon, devStr, device.index());
        qDebug() << "Device Name: " << devStr;
        qDebug() << "Device Id: " << device.index();
    }
    // Print device info:
    std::cout << "Name                        = " << device.name() << std::endl;
    std::cout << "Host API                    = " << device.hostApi().name() << std::endl;
    std::cout << "Max inputs = " << device.maxInputChannels() << ", Max outputs = " << device.maxOutputChannels() << std::endl;

    std::cout << "Default low input latency   = " << device.defaultLowInputLatency() << std::endl; // 8.3
    std::cout << "Default low output latency  = " << device.defaultLowOutputLatency() << std::endl; // 8.3
    std::cout << "Default high input latency  = " << device.defaultHighInputLatency() << std::endl; // 8.3
    std::cout << "Default high output latency = " << device.defaultHighOutputLatency() << std::endl; // 8.3
}

QIcon SetupWindow::getAudioIcon(std::string &hostApiName) {
	if(hostApiName == "Core Audio")
        return iconCoreAudio;
    return QIcon();
}

void SetupWindow::selectAudioDevice(int audioDeviceIndex) {
    if(audioDeviceIndex<0) {
        ui->comboBoxSoundDevices->setCurrentIndex(0);
        return;
    }
    int index;
    int itemAmount = ui->comboBoxSoundDevices->count();
    bool ok;

    for(int i=0; i<itemAmount; i++) {
        index = ui->comboBoxSoundDevices->itemData(i).toInt(&ok);
        if(ok) {
            if(index == audioDeviceIndex) {
                ui->comboBoxSoundDevices->setCurrentIndex(i);
                return;
            }
        }
    }
}

int SetupWindow::getSelectedAudioDeviceIndex() {
    bool ok;
    int index = ui->comboBoxSoundDevices->currentData().toInt(&ok);
    return ok ? index : -1;
}

SoundResolution SetupWindow::getSelectedSoundResolution() {
    SoundResolution soundResolution;
    bool ok = false;
    int currentData = ui->bitDepths->currentData().toInt(&ok);
    if(ok) {
        soundResolution.bitDepth = (BitDepth) currentData;
    }
    currentData = ui->samplingFrequencies->currentData().toInt(&ok);
    if(ok) {
        soundResolution.sampleRate = (SamplingFrequency) currentData;
    }
    currentData = ui->channels->currentData().toInt(&ok);
    if(ok) {
        soundResolution.channelMode = (ChannelMode) currentData;
    }
    return soundResolution;
}

void SetupWindow::selectSampleRate(SamplingFrequency samplingFrequency) {
    for(int i = 0; i < ui->samplingFrequencies->count(); i++) {
        if(ui->samplingFrequencies->itemData(i).toInt() == (int) samplingFrequency) {
            ui->samplingFrequencies->setCurrentIndex(i);
            break;
        }
    }
}

void SetupWindow::selectBitDepth(BitDepth bitRate) {
    for(int i = 0; i < ui->bitDepths->count(); i++) {
        if(ui->bitDepths->itemData(i).toInt() == (int) bitRate) {
            ui->bitDepths->setCurrentIndex(i);
            break;
        }
    }
}

void SetupWindow::selectChannelMode(ChannelMode channelMode) {
    for(int i = 0; i < ui->channels->count(); i++) {
        if(ui->channels->itemData(i).toInt() == (int) channelMode) {
            ui->channels->setCurrentIndex(i);
            break;
        }
    }
}

void SetupWindow::on_checkBoxSaveSettingsImmediately_toggled(bool checked) {
    emit MessageCenter::getInstance().requests.settingsRequests.settingsSavingImmediatelyModeChangeRequested(checked);
}

void SetupWindow::onSettingsSavingImmediatelyModeChangeRequested(bool saveImmediately) {
    immediateMode = saveImmediately;
    if(saveImmediately) {
        ui->buttonBox->hide();
    }
    else {
        SettingsCenter::getInstance().getParameters()->save();

        ui->buttonBox->show();
    }
    emit MessageCenter::getInstance().events.settingsEvents.settingsSavingImmediatelyModeChanged(saveImmediately);
}

void SetupWindow::on_checkBoxSaveSettingsImmediately_clicked(bool checked) {
    if(checked) {
        save();
    }
}

void SetupWindow::on_checkBoxHideTitleBar_toggled(bool checked) {
    if(immediateMode) {
        emit MessageCenter::getInstance().requests.windowRequests.titleBarHidingStateChangeRequested(checked);
    }
}

void SetupWindow::on_treeMenu_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    if(current->text(1) == "General")
        ui->pages->setCurrentWidget(ui->generalPage);
    else if(current->text(1) == "Audio")
        ui->pages->setCurrentWidget(ui->audioPage);
    else if(current->text(1) == "Player")
        ui->pages->setCurrentWidget(ui->playerPage);
    else if(current->text(1) == "DSP")
        ui->pages->setCurrentWidget(ui->dspPage);
    else if(current->text(1) == "Directories")
        ui->pages->setCurrentWidget(ui->directoriesPage);
    else if(current->text(1) == "Visualization")
        ui->pages->setCurrentWidget(ui->visualizationPage);
    else if(current->text(1) == "Oscilloscope")
        ui->pages->setCurrentWidget(ui->oscilloscopePage);
    else if(current->text(1) == "SpectrumAnalyzer")
        ui->pages->setCurrentWidget(ui->spectrumAnalyzerPage);
    else if(current->text(1) == "VUMeter")
        ui->pages->setCurrentWidget(ui->vuMeterPage);
    else if(current->text(1) == "Display")
        ui->pages->setCurrentWidget(ui->displayPage);
    else if(current->text(1) == "Window")
        ui->pages->setCurrentWidget(ui->windowPage);
    else if(current->text(1) == "Colors")
        ui->pages->setCurrentWidget(ui->colorsPage);
    else if(current->text(1) == "PlugIns")
        ui->pages->setCurrentWidget(ui->pluginsPage);
}

void SetupWindow::on_comboBox_spectrumAnalyzerType_currentIndexChanged(int index) {
    if(index == 0) {
        ui->labelSpectrumAnalyzerLedAmount->show();
        ui->label_ledAmount_Text->show();
        ui->spectrumAnalyzerLedAmount->show();
        ui->spectrumAnalyzerLedHeightRatioLabel->show();
        ui->label_ledSize_Text->show();
        ui->spectrumAnalyzerLedHeightRatio->show();

        ui->label_peakHeight->hide();
        ui->label_peakHeight_text->hide();
        ui->horizontalSlider_peakHeight->hide();
    }
    else if(index == 1) {
        ui->labelSpectrumAnalyzerLedAmount->hide();
        ui->label_ledAmount_Text->hide();
        ui->spectrumAnalyzerLedAmount->hide();
        ui->spectrumAnalyzerLedHeightRatioLabel->hide();
        ui->label_ledSize_Text->hide();
        ui->spectrumAnalyzerLedHeightRatio->hide();

        if(ui->checkBox_showSpectrumAnalyzerPeaks->isChecked()) {
            ui->label_peakHeight->show();
            ui->label_peakHeight_text->show();
            ui->horizontalSlider_peakHeight->show();
        }
    }
}

void SetupWindow::on_checkBox_showPeaks_stateChanged(int checked) {
    if(checked) {
        ui->label_peakTimeout->show();
        ui->label_peakTimeout_text->show();
        ui->horizontalSlider_peakTimeout->show();
        if(ui->spectrumAnalyzerType->currentIndex() == 1) {
            ui->label_peakHeight->show();
            ui->label_peakHeight_text->show();
            ui->horizontalSlider_peakHeight->show();
        }
    }
    else {
        ui->label_peakTimeout->hide();
        ui->label_peakTimeout_text->hide();
        ui->horizontalSlider_peakTimeout->hide();
        ui->label_peakHeight->hide();
        ui->label_peakHeight_text->hide();
        ui->horizontalSlider_peakHeight->hide();
    }
}

void SetupWindow::on_horizontalSlider_peakTimeout_valueChanged(int value) {
    ui->label_peakTimeout->setText(QString::number(value*250) + " ms");
}

void SetupWindow::on_checkBoxUseSpectrumAnalyzerSettings_toggled(bool checked) {
    ui->tabWidgetVuMeter->setHidden(checked);
}

void SetupWindow::on_comboBoxImagePlacement_currentIndexChanged(int index) {
    ui->aheadTheSignalWarning->setHidden(index == 0);
}

void SetupWindow::on_comboBoxOscilloscopeSignalColorType_currentIndexChanged(int index) {
    ui->oscilloscopeSignalGradient->setHidden(index != 1);
    ui->oscilloscopeSignalColor->setHidden(index != 0);
}

void SetupWindow::on_pushButton_RescanDeviceList_clicked() {
    ui->comboBoxSoundDevices->clear();
    initAudioInterfaceList();
}

void SetupWindow::on_comboBoxSoundDevices_currentIndexActivated(int index) {
    bool ok;
    int deviceIndex = ui->comboBoxSoundDevices->itemData(index).toInt(&ok);
    if(ok) {
        MppParameters *parameters = SettingsCenter::getInstance().getParameters();
        parameters->audioDeviceIndex = deviceIndex;
        if(immediateMode) {
            portaudio::System &sys = portaudio::System::instance();
        }
        emit MessageCenter::getInstance().requests.soundRequests.outputDeviceChangeRequested(deviceIndex);
    }
}

void SetupWindow::on_snappingThreshold_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->snappingThreshold = position;
}

void SetupWindow::on_snappingThreshold_valueChanged(int value) {
    ui->snappingThresholdLabel->setText(QString::number(value));
}

void SetupWindow::on_checkBoxSnapToViewPort_toggled(bool checked) {
    ui->groupBoxSnappingThreshold->setEnabled(checked);
}

void SetupWindow::on_spectrumAnalyzerType_currentIndexChanged(int index) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    BarType barType = index == 0 ? BarType::Discrete : BarType::Continuous;
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.barTypeChangeRequested(barType);
    parameters->spectrumAnalyzerType = barType;
}

void SetupWindow::on_spectrumAnalyzerLedAmount_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.barLedAmountChangeRequested(position);
    parameters->spectrumAnalyzerBarLedAmount = position;
}

void SetupWindow::on_spectrumAnalyzerLedAmount_valueChanged(int value) {
    ui->labelSpectrumAnalyzerLedAmount->setText(QString::number(value));
}

void SetupWindow::on_spectrumAnalyzerBarWidthRatio_valueChanged(int value) {
    ui->labelSpectrumAnalyzerBarRatio->setText(QString::number(value) + "%");
}

void SetupWindow::on_spectrumAnalyzerBarWidthRatio_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    double barWidthRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.barWidthRatioChangeRequested(barWidthRatio);
    parameters->spectrumAnalyzerBarWidthRatio = barWidthRatio;
}

void SetupWindow::on_spectrumAnalyzerLedHeightRatio_valueChanged(int value) {
    ui->spectrumAnalyzerLedHeightRatioLabel->setText(QString::number(value) + "%");
}

void SetupWindow::on_spectrumAnalyzerLedHeightRatio_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    double ledHeightRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.ledHeightRatioChangeRequested(ledHeightRatio);
    parameters->spectrumAnalyzerLedHeightRatio = ledHeightRatio;
}

void SetupWindow::on_spectrumAnalyzerBarAmount_valueChanged(int value) {
    ui->spectrumAnalyzerBarAmountLabel->setText(QString::number(value));
}

void SetupWindow::on_spectrumAnalyzerBarAmount_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.barAmountChangeRequested(position);
    parameters->spectrumAnalyzerBarAmount = position;
}

void SetupWindow::on_spectrumAnalyzerWindowFunction_currentIndexChanged(int index) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    WindowFunction windowFunction = static_cast<WindowFunction>(index);

    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.windowFunctionChangeRequested(windowFunction);
    parameters->spectrumAnalyzerWindowFunction = windowFunction;
}

void SetupWindow::on_spectrumAnalyzerDimmingRatio_valueChanged(int value) {
    if(value < 0)
        ui->spectrumAnalyzerDimmingRatioLabel->setText(QString::number(-value) + "% Darker");
    else if(value == 0)
        ui->spectrumAnalyzerDimmingRatioLabel->setText("No Dimming");
    if(value > 0)
        ui->spectrumAnalyzerDimmingRatioLabel->setText(QString::number(value) + "% Lighter");
}

void SetupWindow::on_spectrumAnalyzerDimmingRatio_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    double dimmingRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.dimmingRatioChangeRequested(dimmingRatio);
    parameters->spectrumAnalyzerDimmingRatio = dimmingRatio;
}

void SetupWindow::on_spectrumAnalyzerDimmedTransparencyRatio_valueChanged(int value) {
    ui->spectrumAnalyzerDimmedTransparencyRatioLabel->setText(QString::number(value) + "%");
}

void SetupWindow::on_spectrumAnalyzerDimmedTransparencyRatio_sliderMoved(int position) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    double dimmedTransparencyRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.dimmedTransparencyRatioChangeRequested(dimmedTransparencyRatio);
    parameters->spectrumAnalyzerDimmedTransparencyRatio = dimmedTransparencyRatio;
}

void SetupWindow::on_checkBox_showSpectrumAnalyzerPeaks_toggled(bool checked) {
    ui->groupBoxSpectrumAnalyzerPeak->setEnabled(checked);
}

void SetupWindow::closeEvent(QCloseEvent * bar) {
    if(immediateMode) {
        emit MessageCenter::getInstance().requests.settingsRequests.settingsSaveRequested();
    }
}

void SetupWindow::on_spectrumAnalyzerMaximumValue_valueChanged(int value) {
    ui->spectrumAnalyzerMaximumValueLabel->setText(QString::number(value));
}

void SetupWindow::on_spectrumAnalyzerMaximumValue_sliderMoved(int position) {
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.maximumValueChangeRequested(position);
}

void SetupWindow::onSpectrumAnalyzerColorRampChanged() {
    QGradientStops gradient = ui->spectrumAnalyzerColorRampEditor->getColorRamp();
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.gradientChangeRequested(gradient);
}

void SetupWindow::on_vuMeterType_currentIndexChanged(int index) {
    BarType barType = index == 0 ? BarType::Discrete : BarType::Continuous;
    emit MessageCenter::getInstance().requests.vuMeterRequests.barTypeChangeRequested(barType);
}

void SetupWindow::on_vuMeterMaximumValue_valueChanged(int value) {
    ui->vuMeterMaximumValueLabel->setText(QString::number(value));
}

void SetupWindow::on_vuMeterMaximumValue_sliderMoved(int position) {
    emit MessageCenter::getInstance().requests.vuMeterRequests.maximumValueChangeRequested(position);
}

void SetupWindow::on_vuMeterMinimumValue_valueChanged(int value) {
    ui->vuMeterMinimumValueLabel->setText(QString::number(value));
}

void SetupWindow::on_vuMeterMinimumValue_sliderMoved(int position) {
    emit MessageCenter::getInstance().requests.vuMeterRequests.minimumValueChangeRequested(position);
}

void SetupWindow::on_vuMeterLedAmount_valueChanged(int value) {
    ui->vuMeterLedAmountLabel->setText(QString::number(value));
}

void SetupWindow::on_vuMeterLedAmount_sliderMoved(int position) {
    emit MessageCenter::getInstance().requests.vuMeterRequests.barLedAmountChangeRequested(position);
}

void SetupWindow::on_vuMeterLedHeightRatio_valueChanged(int value) {
    ui->vuMeterLedHeightRatioLabel->setText(QString::number(value) + "%");
    qDebug()<<value;
}

void SetupWindow::on_vuMeterLedHeightRatio_sliderMoved(int position) {
    double ledHeightRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.vuMeterRequests.ledHeightRatioChangeRequested(ledHeightRatio);
}

void SetupWindow::on_vuMeterDimmingRatio_valueChanged(int value) {
    if(value < 0)
        ui->vuMeterDimmingRatioLabel->setText(QString::number(-value) + "% Darker");
    else if(value == 0)
        ui->vuMeterDimmingRatioLabel->setText("No Dimming");
    if(value > 0)
        ui->vuMeterDimmingRatioLabel->setText(QString::number(value) + "% Lighter");
}

void SetupWindow::on_vuMeterDimmingRatio_sliderMoved(int position) {
    double dimmingRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.vuMeterRequests.dimmingRatioChangeRequested(dimmingRatio);
}

void SetupWindow::on_vuMeterDimmedTransparencyRatio_valueChanged(int value) {
    ui->vuMeterDimmedTransparencyRatioLabel->setText(QString::number(value) + "%");
}

void SetupWindow::on_vuMeterDimmedTransparencyRatio_sliderMoved(int position) {
    double dimmedTransparencyRatio = double(position)/double(100);
    emit MessageCenter::getInstance().requests.vuMeterRequests.dimmedTransparencyRatioChangeRequested(dimmedTransparencyRatio);
}

void SetupWindow::onVuMeterColorRampChanged() {
    QGradientStops gradient = ui->vuMeterColorRampEditor->getColorRamp();
    emit MessageCenter::getInstance().requests.vuMeterRequests.gradientChangeRequested(gradient);
}

void SetupWindow::on_spectrumAnalyzerLogarithmicScale_checkStateChanged(const Qt::CheckState &checkState) {
    bool isLogarithmic = (checkState == Qt::CheckState::Checked);
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.scaleTypeChangeRequested(isLogarithmic);
}

void SetupWindow::onBitDepthsComboBoxCurrentIndexChanged(int index) {
    SoundResolution soundResolution = getSelectedSoundResolution();
    emit MessageCenter::getInstance().requests.soundRequests.soundResolutionChangeRequested(soundResolution);
}

void SetupWindow::onSamplingFrequenciesComboBoxCurrentIndexChanged(int index) {
    SoundResolution soundResolution = getSelectedSoundResolution();
    emit MessageCenter::getInstance().requests.soundRequests.soundResolutionChangeRequested(soundResolution);
}

void SetupWindow::onChannelsComboBoxCurrentIndexChanged(int index) {
    SoundResolution soundResolution = getSelectedSoundResolution();
    emit MessageCenter::getInstance().requests.soundRequests.soundResolutionChangeRequested(soundResolution);
}

void SetupWindow::onDithersComboBoxCurrentIndexChanged(int index) {
    SoundResolution soundResolution = getSelectedSoundResolution();
    emit MessageCenter::getInstance().requests.soundRequests.soundResolutionChangeRequested(soundResolution);
}
