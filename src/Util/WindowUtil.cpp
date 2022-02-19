#include "WindowUtil.hpp"
#ifdef Q_OS_MACOS
    #include "MacManager.h"
#endif
void WindowUtil::setAlwaysOnTop(QMainWindow *window, bool alwaysOnTop) {
    #ifdef Q_OS_MACOS
            MacManager::toggleAlwaysOnTop(window->winId(), alwaysOnTop);
    #elif defined(Q_OS_WIN)
            // #include <windows.h>
            if (alwaysOnTop)
            {
                window->setWindowFlags(window->windowFlags() | Qt::WindowStaysOnTopHint);
            }
            else
            {
                window->setWindowFlags(window->windowFlags() & ~Qt::WindowStaysOnTopHint);
            }
    #else
            if (alwaysOnTop)
            {
                window->setWindowFlags(window->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
                window->show();
            }
            else
            {
                window->setWindowFlags(window->windowFlags() ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
                window->show();
            }
    #endif
}

void WindowUtil::setAlwaysOnTop(QDialog * dialog, bool alwaysOnTop)
{
    dialog->setWindowFlag(Qt::WindowStaysOnTopHint, alwaysOnTop);
    dialog->show();
}
