/*
Main function definition of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "PlayListEditorWindow.hpp"
#include <QtGlobal>
#include <QApplication>
#include <QFontDatabase>
#include "PlayingCenter.hpp"
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif

#include <EventFilters.hpp>

Q_DECLARE_METATYPE(InterpolationFilter);
Q_DECLARE_METATYPE(RepeatMode)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/Fonts/Seven Segment.ttf");
    QFontDatabase::addApplicationFont(":/Fonts/Inter.ttf");
    PlayingCenter playingCenter;
    PlayerWindow playerWindow;
    PlayListEditorWindow playListEditorWindow;
    app.installEventFilter(new EventFilters::ShowByApplicationActivateEventFilter(&playerWindow));
	//window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    /*
    window.setAttribute(Qt::WA_TranslucentBackground, true);
    */
    #ifdef Q_OS_MACOS
        MacManager::removeTitlebarFromWindow(playerWindow.winId());
        MacManager::nonFunctionalZoomButton(playerWindow.winId());
		//MacManager::setMovableByWindowBackground(window.winId(), true);
	#endif
    playerWindow.show();
    return app.exec();
}
