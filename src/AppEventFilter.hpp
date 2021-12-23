/*
AppEventFilter class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef APPEVENTFILTER_HPP
#define APPEVENTFILTER_HPP
#include <QObject>
#include <QMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QMainWindow>

class AppEventFilter : public QObject
{
		Q_OBJECT
	public:
		AppEventFilter(QMainWindow * mainWindow);
	protected:
		bool eventFilter(QObject* object, QEvent* event);
		QMainWindow *mainWindow;
};

#endif // APPEVENTFILTER_HPP
