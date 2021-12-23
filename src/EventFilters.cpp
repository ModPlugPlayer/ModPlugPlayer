/*
EventFilters class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "EventFilters.hpp"
#include <QMouseEvent>
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif

MoveByMouseClickEventFilter::MoveByMouseClickEventFilter(QMainWindow *mainWindow)
{
	this->mainWindow = mainWindow;
}

bool MoveByMouseClickEventFilter::eventFilter(QObject *watched, QEvent *event)
{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
			if (mouse_event->button() == Qt::LeftButton)
			{
                dragPosition = mouse_event->globalPosition().toPoint() - mainWindow->frameGeometry().topLeft();
				return false;
			}
		}
		else if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
			if (mouse_event->buttons() & Qt::LeftButton)
			{
                mainWindow->move(mouse_event->globalPosition().toPoint() - dragPosition);
				return false;
			}
		}

	return false;
}

KeepFixedSizeEventFilter::KeepFixedSizeEventFilter(QMainWindow *mainWindow)
{
	this->mainWindow = mainWindow;
}

bool KeepFixedSizeEventFilter::eventFilter(QObject *watched, QEvent *event)
{
	if(event->type() == QEvent::LayoutRequest) {
		mainWindow->setFixedSize(mainWindow->sizeHint());
		#ifdef Q_OS_MACOS
		MacManager::enableButton(mainWindow->winId(), true, true, true);
		#endif
	}

	return false;
}
