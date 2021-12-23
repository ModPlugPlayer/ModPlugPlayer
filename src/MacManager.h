/*
MacManager class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MACMANAGER_HPP
#define MACMANAGER_HPP
#include <QGuiApplication>
#include <QWindow>

class MacManager
{
	public:
		static void removeTitlebarFromWindow(long winId = -1);
		static void hideZoomButton(long winId = -1);
		static void disableZoomButton(long winId = -1);
		static void nonFunctionalZoomButton(long winId = -1);
		static void hideButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton);
		static void showButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton);
		static void disableButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton);
		static void enableButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton);
		static void toggleAlwaysOnTop(long winId, bool alwaysOnTop);
		static void setMovableByWindowBackground(long winId, bool movable);
};

#endif // MACMANAGER_HPP
