/*
PlayListEditorWindow class definitions for the test purposes of PlayListWidget
Copyright (C) 2022 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "PlayListEditorWindow.hpp"
#include "./ui_PlayListEditorWindow.h"
#include "PlayerWindow.hpp"
#include <QObject>
#include <boost/uuid/uuid_generators.hpp>

PlayListEditorWindow::PlayListEditorWindow(QWidget *parent, Player *playerWindow)
    : QMainWindow(parent)
    , ui(new Ui::PlayListEditorWindow)
{
    ui->setupUi(this);
    this->playerWindow = playerWindow;
    connect(ui->playListWidget, &PlayListWidget::fileDropped, this, &PlayListEditorWindow::onFileDropped);
    connect(ui->playListWidget, &PlayListWidget::filesDropped, this, &PlayListEditorWindow::onFilesDropped);
    connect(ui->ClearList, &QPushButton::clicked, ui->playListWidget, &PlayListWidget::clear);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::previous, ui->playListWidget, &PlayListWidget::onPreviousSong);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::next, ui->playListWidget, &PlayListWidget::onNextSong);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::repeatModeChanged, ui->playListWidget, &PlayListWidget::onRepeat);
    connect(ui->playListWidget, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::play),(PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::onPlayRequested));
    connect((PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::openRequested), ui->playListWidget, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::onOpen));
    connect((PlayerWindow *) playerWindow, &PlayerWindow::previous, this, &PlayListEditorWindow::onPlayPrevious);
    connect((PlayerWindow *) playerWindow, &PlayerWindow::next, this, &PlayListEditorWindow::onPlayNext);
    ui->playListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    windowGeometry = geometry();
    windowGeometry.setX(parent->geometry().x());
    windowGeometry.setY(parent->geometry().y());
    //ui->playListWidget.model().rowsMoved.connect(lambda: anyfunction())
}

void PlayListEditorWindow::onFileDropped(QUrl fileUrl, int droppedIndex)
{
    PlayListItem item;
    item.id = boost::uuids::random_generator()();
    item.itemNumber = droppedIndex;
    item.filePath = fileUrl.path().toStdString();
    item.title = fileUrl.fileName();
    ui->playListWidget->addPlayListItem(item, droppedIndex);
}

void PlayListEditorWindow::onFilesDropped(QList<QUrl> fileUrls, int droppedIndex)
{
    QList<PlayListItem> items;
    for(QUrl &fileUrl:fileUrls) {
        PlayListItem item;
        item.id = boost::uuids::random_generator()();
        item.itemNumber = droppedIndex;
        item.filePath = fileUrl.path().toStdString();
        item.title = fileUrl.fileName();
        items.append(item);
    }
    ui->playListWidget->addPlayListItems(items, droppedIndex);
}

void PlayListEditorWindow::onPlayPrevious()
{

}

void PlayListEditorWindow::onPlayNext()
{

}

PlayListEditorWindow::~PlayListEditorWindow()
{
    delete ui;
}

void PlayListEditorWindow::closeEvent(QCloseEvent * event)
{
}

void PlayListEditorWindow::showEvent(QShowEvent * event)
{
    setGeometry(windowGeometry);
}

void PlayListEditorWindow::hideEvent(QHideEvent * event)
{
    windowGeometry = geometry();
    emit hidden();
}

void PlayListEditorWindow::on_Add_clicked()
{
    PlayListItem item;
    item.itemNumber = ui->playListWidget->count();
    item.id = boost::uuids::random_generator()();
    ui->playListWidget->addPlayListItem(item);
    //ui->playListWidget->insertItem(ui->playListWidget->count(), QString::number(ui->playListWidget->count()));


    //ui->playListWidget->addItem(QString::number(ui->playListWidget->count()));

}

void PlayListEditorWindow::on_Remove_clicked()
{
    ui->playListWidget->removeSelectedItems();
}
