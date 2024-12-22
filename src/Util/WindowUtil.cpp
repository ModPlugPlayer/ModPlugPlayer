/*
WindowUtil class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include "WindowUtil.hpp"
#include <QFontMetrics>
#include <QStringView>

#ifdef Q_OS_MACOS
    #include "MacManager.h"
#endif
void WindowUtil::setAlwaysOnTop(QMainWindow *window, bool alwaysOnTop) {
    #ifdef Q_OS_MACOS
            MacManager::toggleAlwaysOnTop(window->winId(), alwaysOnTop);
    #elif defined(Q_OS_WIN)
            window->setWindowFlag(Qt::WindowStaysOnTopHint, alwaysOnTop);
            window->show();
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

QString WindowUtil::shortenTextToWidth(const QFont &font, const size_t &width, const QString &text) {
    QFontMetrics fontMetrics(font);
    if(fontMetrics.boundingRect(text).width()<=width)
        return text;

    QString currentText;

    size_t currentSubStringLength = text.size();
    while(true) {
        currentSubStringLength--;
        if(currentSubStringLength < 1)
            return QString("");
        currentText = text.mid(0, currentSubStringLength) + "...";
        if(fontMetrics.boundingRect(currentText).width()<=width)
            return currentText;
    }
    return currentText;
}

QString WindowUtil::shortenTextToWidth(const QFont &font, const size_t &width, const QString &text, const QString &textPostFix) {
    QFontMetrics fontMetrics(font);
    if(fontMetrics.boundingRect(text + textPostFix).width()<=width)
        return text + textPostFix;

    QString currentText;

    size_t currentSubStringLength = text.size();
    while(true) {
        currentSubStringLength--;
        if(currentSubStringLength < 1)
            return QString(textPostFix);
        currentText = text.mid(0, currentSubStringLength) + "..." + textPostFix;
        if(fontMetrics.boundingRect(currentText).width()<=width)
            return currentText;
    }
    return QString(textPostFix);
}
