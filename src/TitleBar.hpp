/*
TitleBar class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TITLEBAR_HPP
#define TITLEBAR_HPP

#include <QWidget>
#include <RGB.hpp>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
	void setActiveColor(const RGB &color);
	RGB getActiveColor();
	void setInactiveColor(const RGB &color);
	RGB getInactiveColor();
    void setTitle(QString title);
	~TitleBar();
signals:
    void minimizeButtonClicked();
    void miniPlayerButtonClicked();
    void closeButtonClicked();
private:
    Ui::TitleBar *ui;
	RGB activeColor, inactiveColor;
	void setStyleSheetColor(RGB color);
};

#endif // TITLEBAR_HPP
