#include "AppEventFilter.hpp"

AppEventFilter::AppEventFilter(QMainWindow * mainWindow){
	this->mainWindow = mainWindow;
}

bool AppEventFilter::eventFilter(QObject* object, QEvent* event) {
	if (event->type() == QEvent::ApplicationActivate) {
		if(!mainWindow->isVisible()) {
			mainWindow->show();
			return false;
		}
	}
	return false;
  }

