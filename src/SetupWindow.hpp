#ifndef SETUPWINDOW_HPP
#define SETUPWINDOW_HPP

#include <QDialog>
#include "MppParameters.hpp"
#include <QAbstractButton>
#include "PlayerWindow.hpp"

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

	void on_listWidget_currentRowChanged(int currentRow);

	void on_stereoSeparationSlider_valueChanged(int value);

	void on_checkBoxEnableSystemTray_toggled(bool checked);

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_checkBoxSaveSettingsImmediately_toggled(bool checked);

	void on_checkBoxHideTitleBar_toggled(bool checked);

	void on_pushButton_SetBufferLengthToDefaultValue_clicked();

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
