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

	void on_stereoSeparationSlider_sliderMoved(int position);

	private:
    Ui::SetupWindow *ui;
};

#endif // SETUPWINDOW_HPP
