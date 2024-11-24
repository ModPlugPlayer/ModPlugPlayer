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

bool MoveByMouseClickEventFilter::eventFilter(QObject * watched, QEvent * event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::LeftButton)
        {
            dragPosition = mouse_event->globalPosition().toPoint() - mainWindow->geometry().topLeft();
            lastPosition = dragPosition;
            qDebug()<<"Drag Pos: "<<dragPosition;
            return false;
        }
    }
    else if (event->type() == QEvent::MouseMove) {

        QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
        if (mouse_event->buttons() & Qt::LeftButton) {
            QPoint moveTo =  mouse_event->globalPosition().toPoint() - dragPosition;
            QPoint movementDelta = moveTo - lastPosition;
            QRect availableScreenGeometry = mainWindow->screen()->availableGeometry();
            QRect availableVirtualScreenGeometry = mainWindow->screen()->availableVirtualGeometry();

            if(keepStayingInViewPort) {
                //Don't exceed virtual screen boundaries
                if(moveTo.x() < availableVirtualScreenGeometry.left())
                    moveTo.setX(availableVirtualScreenGeometry.left());
                if(moveTo.x() + mainWindow->width() > availableVirtualScreenGeometry.right())
                    moveTo.setX(availableVirtualScreenGeometry.right() - mainWindow->width());
                if(moveTo.y() < availableVirtualScreenGeometry.top())
                    moveTo.setY(availableVirtualScreenGeometry.top());
                if(moveTo.y() + mainWindow->height() > availableVirtualScreenGeometry.bottom())
                    moveTo.setY(availableVirtualScreenGeometry.bottom() - mainWindow->height());
            }

            if(snapToViewPort) {
                if(moveTo.x() - availableScreenGeometry.left() > -snappingThreshold && movementDelta.x() < 0) {
                    if(moveTo.x() < availableScreenGeometry.left())
                        moveTo.setX(availableScreenGeometry.left());
                }

                if(availableScreenGeometry.right() - moveTo.x() - mainWindow->width() > -snappingThreshold && movementDelta.x() > 0) {
                    if(moveTo.x() + mainWindow->width() > availableScreenGeometry.right()) {
                        moveTo.setX(availableScreenGeometry.right()-mainWindow->width());
                    }
                }


                if(moveTo.y() - availableScreenGeometry.top() > -snappingThreshold && movementDelta.y() < 0) {
                    if(moveTo.y() < availableScreenGeometry.top())
                        moveTo.setY(availableScreenGeometry.top());
                }

                if(availableScreenGeometry.bottom() - moveTo.y() - mainWindow->height() > -snappingThreshold && movementDelta.y() > 0) {
                    if(moveTo.y() + mainWindow->height() > availableScreenGeometry.bottom()) {
                        moveTo.setY(availableScreenGeometry.bottom()-mainWindow->height());
                    }
                }
            }
            mainWindow->move(moveTo);
            lastPosition = moveTo;

            return false;
        }
    }

    return false;
}

bool MoveByMouseClickEventFilter::getSnapToViewPort() const
{
    return snapToViewPort;
}

void MoveByMouseClickEventFilter::setSnapToViewPort(bool snapToViewPort)
{
    this->snapToViewPort = snapToViewPort;
}

int MoveByMouseClickEventFilter::getSnappingThreshold() const
{
    return snappingThreshold;
}

void MoveByMouseClickEventFilter::setSnappingThreshold(int snappingThreshold)
{
    this->snappingThreshold = snappingThreshold;
}

bool MoveByMouseClickEventFilter::getKeepStayingInViewPort() const
{
    return keepStayingInViewPort;
}

void MoveByMouseClickEventFilter::setKeepStayingInViewPort(bool keepStayingInViewPort)
{
    this->keepStayingInViewPort = keepStayingInViewPort;
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

