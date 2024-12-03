/*
Main function definition of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include <QtGlobal>
#include <QApplication>
#include <QFontDatabase>
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif

#include "AppEventFilter.hpp"

Q_DECLARE_METATYPE(InterpolationFilter);
Q_DECLARE_METATYPE(RepeatMode)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/Fonts/Seven Segment.ttf");
    QFontDatabase::addApplicationFont(":/Fonts/Inter.ttf");
    PlayerWindow window;
	a.installEventFilter(new AppEventFilter(&window));
	//window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    /*
    window.setAttribute(Qt::WA_TranslucentBackground, true);
    */
    #ifdef Q_OS_MACOS
		MacManager::removeTitlebarFromWindow(window.winId());
		MacManager::nonFunctionalZoomButton(window.winId());
		//MacManager::setMovableByWindowBackground(window.winId(), true);
	#endif
    window.show();
    return a.exec();
}
