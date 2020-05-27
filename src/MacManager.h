#ifndef MACMANAGER_HPP
#define MACMANAGER_HPP
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
		static void toggleAlwaysOnTop(long winId, bool alwaysOnTop);
		static void setMovableByWindowBackground(long winId, bool movable);
};

#endif // MACMANAGER_HPP
