#include "EventFilters.hpp"
#include <QMouseEvent>

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
				dragPosition = mouse_event->globalPos() - mainWindow->frameGeometry().topLeft();
				return false;
			}
		}
		else if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
			if (mouse_event->buttons() & Qt::LeftButton)
			{
				mainWindow->move(mouse_event->globalPos() - dragPosition);
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
	}

	return false;
}

