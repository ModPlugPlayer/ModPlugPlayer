#include <QGuiApplication>
#include <QWindow>

class MacManager
{
	public:
		static void removeTitlebarFromWindow(long winId = -1);
		static void hideZoomButton(long winId = -1);
		static void disableZoomButton(long winId = -1);
		static void nonFunctionalZoomButton(long winId = -1);
};
