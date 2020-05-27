#ifndef EVENTFILTERS_HPP
#define EVENTFILTERS_HPP
#include <QObject>
#include <QEvent>
#include <QPoint>
#include <QMainWindow>

class MoveByMouseClickEventFilter : public QObject
{
	Q_OBJECT
	public:
		MoveByMouseClickEventFilter(QMainWindow *mainWindow);
		bool eventFilter(QObject *watched, QEvent *event);
	private:
		QMainWindow *mainWindow;
		QPoint dragPosition;
};

class KeepFixedSizeEventFilter : public QObject
{
	Q_OBJECT
	public:
		KeepFixedSizeEventFilter(QMainWindow *mainWindow);
		bool eventFilter(QObject *watched, QEvent *event);
	private:
		QMainWindow *mainWindow;
};

#endif // EVENTFILTERS_HPP
