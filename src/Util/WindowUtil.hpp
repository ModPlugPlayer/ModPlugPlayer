#pragma once
#include <QDialog>
#include <QMainWindow>

namespace WindowUtil {
    void setAlwaysOnTop(QMainWindow *window, bool alwaysOnTop);
    void setAlwaysOnTop(QDialog *dialog, bool alwaysOnTop);
};

