#include "PlayerWindow.hpp"
#include <QtGlobal>
#include <QApplication>
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif

#include "AppEventFilter.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlayerWindow window;
	a.installEventFilter(new AppEventFilter(&window));
	//window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    /*
    window.setAttribute(Qt::WA_TranslucentBackground, true);
    */
    #ifdef Q_OS_MACOS
		MacManager::removeTitlebarFromWindow(window.winId());
		MacManager::nonFunctionalZoomButton(window.winId());
    #endif
    window.show();
    return a.exec();
}
