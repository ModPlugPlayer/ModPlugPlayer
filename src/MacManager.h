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
};
