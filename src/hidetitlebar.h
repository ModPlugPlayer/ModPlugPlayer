/*
HideTitleBar class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HIDETITLEBAR_H
#define HIDETITLEBAR_H

#include <QWindow>

class HideTitleBar : public QObject
{
    Q_OBJECT
public:
    HideTitleBar();

public slots:
    void hideTitleBar(QWindow* window);
};

#endif // HIDETITLEBAR_H
