/*
AppEventFilter class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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
