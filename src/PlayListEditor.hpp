/*
PlayerListEditor class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QShowEvent>
#include <QHideEvent>

namespace Ui {
class PlayListEditor;
}

class PlayListEditor : public QDialog
{
    Q_OBJECT
    public:
        explicit PlayListEditor(QWidget *parent = nullptr);
        ~PlayListEditor();
    signals:
        void hidden();
    private:
        Ui::PlayListEditor *ui;
        void closeEvent(QCloseEvent *event);
        void showEvent(QShowEvent *event);
        void hideEvent(QHideEvent *event);
        QRect windowGeometry;
};

