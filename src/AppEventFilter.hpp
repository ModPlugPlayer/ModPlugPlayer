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
