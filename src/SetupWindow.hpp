#ifndef SETUPWINDOW_HPP
#define SETUPWINDOW_HPP

#include <QDialog>

namespace Ui {
class SetupWindow;
}

class SetupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetupWindow(QWidget *parent = nullptr);
    ~SetupWindow();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

	void on_stereoSeparationSlider_valueChanged(int value);

	void on_checkBoxEnableSystemTray_toggled(bool checked);

	private:
    Ui::SetupWindow *ui;
};

#endif // SETUPWINDOW_HPP
