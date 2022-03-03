/*
PlayerListEditor class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayListEditor.hpp"
#include "ui_PlayListEditor.h"
#include <QAbstractItemView>

PlayListEditor::PlayListEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayListEditor)
{
    ui->setupUi(this);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    windowGeometry = geometry();
    windowGeometry.setX(parent->geometry().x());
    windowGeometry.setY(parent->geometry().y());
    //ui->listWidget.model().rowsMoved.connect(lambda: anyfunction())
}

PlayListEditor::~PlayListEditor()
{
    delete ui;
}

void PlayListEditor::closeEvent(QCloseEvent * event)
{
}

void PlayListEditor::showEvent(QShowEvent * event)
{
    setGeometry(windowGeometry);
}

void PlayListEditor::hideEvent(QHideEvent * event)
{
    windowGeometry = geometry();
    emit hidden();
}
