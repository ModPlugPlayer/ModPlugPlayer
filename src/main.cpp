#include "PlayerWindow.hpp"
#include <QtGlobal>
#include <QApplication>
#ifdef Q_OS_MACOS
#include <MacManager.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerWindow window;
    //window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    /*
    window.setAttribute(Qt::WA_TranslucentBackground, true);
    */
    #ifdef Q_OS_MACOS
        MacManager::removeTitlebarFromWindow();
    #endif
    window.show();
    return a.exec();
}
