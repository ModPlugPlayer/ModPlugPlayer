#ifndef SETUPWINDOW_HPP
#define SETUPWINDOW_HPP

#include <QDialog>
#include "MppParameters.hpp"
#include <QAbstractButton>
#include "PlayerWindow.hpp"
#include <QTreeWidget>

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

    void on_horizontalSlider_ledAmount_sliderMoved(int position);

    void on_comboBox_spectrumAnalyzerType_currentIndexChanged(int index);

    void on_checkBox_showPeaks_stateChanged(int arg1);

    void on_horizontalSlider_peakTimeout_valueChanged(int value);

    void on_checkBoxUseSpectrumAnalyzerSettings_toggled(bool checked);

    void on_comboBoxImagePlacement_currentIndexChanged(int index);

    void on_comboBoxOscilloscopeSignalColorType_currentIndexChanged(int index);

private:
    Ui::SetupWindow *ui;
	MppParameters *parameters;
	PlayerWindow *playerWindow;
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
	void initAudioIcons();
	void initAudioInterfaceList();
	QIcon getAudioIcon(std::string &hostApiName);
};

#endif // SETUPWINDOW_HPP
