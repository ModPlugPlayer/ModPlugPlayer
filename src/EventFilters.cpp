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

