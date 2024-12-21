/*
SetupWindow class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SETUPWINDOW_HPP
#define SETUPWINDOW_HPP

#include <QDialog>
#include "MppParameters.hpp"
#include <QAbstractButton>
#include "PlayerWindow.hpp"
#include <QTreeWidget>
#include <portaudiocpp/PortAudioCpp.hxx>

namespace Ui {
class SetupWindow;
}

class SetupWindow : public QDialog
{
    Q_OBJECT

public:
	explicit SetupWindow(MppParameters *mppParameters, PlayerWindow *parent = nullptr);
    ~SetupWindow();

public slots:

private slots:

	void onActiveTitleBarTextColorChanged();
	void onInactiveTitleBarTextColorChanged();
	void onActiveButtonLightColorChanged();
	void onInactiveButtonLightColorChanged();
	void onPlayerBodyTextColorChanged();
	void onPlayerBodyBackgroundColorChanged();
	void onLcdDisplayForegroundColorChanged();
	void onLcdDisplayBackgroundColorChanged();

    void on_stereoSeparationSlider_valueChanged(int value);

	void on_checkBoxEnableSystemTray_toggled(bool checked);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_checkBoxSaveSettingsImmediately_toggled(bool checked);

	void on_checkBoxHideTitleBar_toggled(bool checked);

    void on_treeMenu_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_comboBox_spectrumAnalyzerType_currentIndexChanged(int index);

    void on_checkBox_showPeaks_stateChanged(int arg1);

    void on_horizontalSlider_peakTimeout_valueChanged(int value);

    void on_checkBoxUseSpectrumAnalyzerSettings_toggled(bool checked);

    void on_comboBoxImagePlacement_currentIndexChanged(int index);

    void on_comboBoxOscilloscopeSignalColorType_currentIndexChanged(int index);

    void on_pushButton_RescanDeviceList_clicked();

    void on_comboBoxSoundDevices_currentIndexActivated(int index);

    void on_snappingThreshold_sliderMoved(int position);

    void on_snappingThreshold_valueChanged(int value);

    void on_checkBoxSnapToViewPort_toggled(bool checked);

    void on_spectrumAnalyzerType_currentIndexChanged(int index);

    void on_spectrumAnalyzerLedAmount_sliderMoved(int position);

    void on_spectrumAnalyzerLedAmount_valueChanged(int value);

    void on_spectrumAnalyzerBarWidthRatio_valueChanged(int value);

    void on_spectrumAnalyzerBarWidthRatio_sliderMoved(int position);

    void on_spectrumAnalyzerLedHeightRatio_valueChanged(int value);

    void on_spectrumAnalyzerLedHeightRatio_sliderMoved(int position);

    void onSpectrumAnalyzerBandAmountValueChanged(int value);

    void on_checkBoxSaveSettingsImmediately_clicked(bool checked);

    void on_spectrumAnalyzerWindowFunction_currentIndexChanged(int index);

    void on_spectrumAnalyzerDimmingRatio_valueChanged(int value);

    void on_spectrumAnalyzerDimmingRatio_sliderMoved(int position);

    void on_spectrumAnalyzerDimmedTransparencyRatio_valueChanged(int value);

    void on_spectrumAnalyzerDimmedTransparencyRatio_sliderMoved(int position);

    void on_checkBox_showSpectrumAnalyzerPeaks_toggled(bool checked);

    void on_spectrumAnalyzerMaximumValue_valueChanged(int value);

    void on_spectrumAnalyzerMaximumValue_sliderMoved(int position);

    void onSpectrumAnalyzerColorRampChanged();

    void on_vuMeterType_currentIndexChanged(int index);

    void on_vuMeterMaximumValue_valueChanged(int value);

    void on_vuMeterMaximumValue_sliderMoved(int position);

    void on_vuMeterMinimumValue_valueChanged(int value);

    void on_vuMeterMinimumValue_sliderMoved(int position);

    void on_vuMeterLedAmount_valueChanged(int value);

    void on_vuMeterLedAmount_sliderMoved(int position);

    void on_vuMeterLedHeightRatio_valueChanged(int value);

    void on_vuMeterLedHeightRatio_sliderMoved(int position);

    void on_vuMeterDimmingRatio_valueChanged(int value);

    void on_vuMeterDimmingRatio_sliderMoved(int position);

    void on_vuMeterDimmedTransparencyRatio_valueChanged(int value);

    void on_vuMeterDimmedTransparencyRatio_sliderMoved(int position);

    void onVuMeterColorRampChanged();

    void on_spectrumAnalyzerLogarithmicScale_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::SetupWindow *ui;
	MppParameters *parameters;
	PlayerWindow *playerWindow;
    void closeEvent(QCloseEvent *bar) override;
    void connectSignalsAndSlots();
	void load();
	void save();
	void restoreDefaults();
	QIcon iconCoreAudio;
	QIcon iconWdmAudio;
	QIcon iconDirectXAudio;
	QIcon iconAlsaAudio;
	QIcon iconOssAudio;
	QIcon iconAsioAudio;
	QIcon iconJackAudio;
    QIcon emptyIcon;
	void initAudioIcons();
	void initAudioInterfaceList();
    void addDeviceToDeviceList(portaudio::Device &device);
	QIcon getAudioIcon(std::string &hostApiName);
    void selectAudioDevice(int audioDeviceIndex);
    int getSelectedAudioDeviceIndex();
    bool immediateMode;
    void getBandInfoFromBandAmountSliderPosition(const int &position, size_t *bandAmount, size_t *octaveBandFractionDenominator);
};

#endif // SETUPWINDOW_HPP
