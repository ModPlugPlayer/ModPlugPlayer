#pragma once

#include <QDialog>

namespace Ui {
class VersionInfoWindow;
}

class VersionInfoWindow : public QDialog
{
    Q_OBJECT
    public:
    explicit VersionInfoWindow(QWidget *parent = nullptr);
    ~VersionInfoWindow();

private:
    Ui::VersionInfoWindow *ui;
};

