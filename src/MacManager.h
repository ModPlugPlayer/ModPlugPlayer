#include <QGuiApplication>
#include <QWindow>

class MacManager
{
public:
    static void removeTitlebarFromWindow(long winId = -1);
	static void hideFullscreenButton(long winId = -1);
};
