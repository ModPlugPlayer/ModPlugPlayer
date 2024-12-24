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
#include "MPPExceptions.hpp"

PlayListEditorWindow::PlayListEditorWindow(QWidget *parent, Player *playerWindow)
    : QMainWindow(parent)
    , ui(new Ui::PlayListEditorWindow)
{
    ui->setupUi(this);
    this->playerWindow = playerWindow;
    connectSignalsAndSlots();
    ui->playListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    //ui->playListWidget.model().rowsMoved.connect(lambda: anyfunction())
}

void PlayListEditorWindow::connectSignalsAndSlots() {
    connect(ui->playListWidget, &PlayListWidget::fileDropped, this, &PlayListEditorWindow::onFileDropped);
    connect(ui->playListWidget, &PlayListWidget::filesDropped, this, &PlayListEditorWindow::onFilesDropped);
    connect(ui->ClearList, &QPushButton::clicked, ui->playListWidget, &PlayListWidget::clear);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::previous, ui->playListWidget, &PlayListWidget::onPreviousRequested);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::next, ui->playListWidget, &PlayListWidget::onNextRequested);
    connect((PlayerWindow *) this->playerWindow, &PlayerWindow::repeatModeChanged, ui->playListWidget, &PlayListWidget::onRepeatModeChanged);
    connect(ui->playListWidget, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::playRequested),(PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::onPlayRequested));
    //connect((PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::openRequested), ui->playListWidget, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::onOpen));
    connect((PlayerWindow *) playerWindow, &PlayerWindow::previous, this, &PlayListEditorWindow::onPlayPrevious);
    connect((PlayerWindow *) playerWindow, &PlayerWindow::next, this, &PlayListEditorWindow::onPlayNext);
}

PlayListItem createPlayListItemObject(const std::filesystem::path &path, int droppedIndex = 0) {
    PlayListItem item;
    ModuleFileMetadataReader metaDataReader(path);
    ModuleFileInfo modInfo = metaDataReader.getModuleFileInfo();
    item.id = modInfo.id;
    item.itemNumber = droppedIndex;
    item.filePath = modInfo.filePath;
    item.title = modInfo.moduleInfo.songTitle.c_str();
    item.format = QString(modInfo.moduleInfo.moduleFormat.c_str()).toUpper();
    item.duration = modInfo.moduleInfo.songDuration;
    return item;
}

void PlayListEditorWindow::addFileOrFolderToPlayList(const std::filesystem::path &path, int &droppedIndex) {
    if(!std::filesystem::exists(path))
        return;
    if(std::filesystem::is_directory(path)) {
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            addFileOrFolderToPlayList(entry.path(), droppedIndex);
        }
    }
    try {
        PlayListItem playListItem = createPlayListItemObject(path, droppedIndex);
        ui->playListWidget->addPlayListItem(playListItem, droppedIndex);
        droppedIndex++;
    } catch (Exceptions::UnsupportedFileFormatException e) {
        // To-do: Add error message here
        qDebug()<<"Unsupported File";
    }
}

void PlayListEditorWindow::onFileDropped(QUrl fileUrl, int droppedIndex)
{
    std::filesystem::path path(fileUrl.path().toStdString());
    addFileOrFolderToPlayList(path, droppedIndex);
    ui->playListWidget->updateItemNumbers();
}

void PlayListEditorWindow::onFilesDropped(QList<QUrl> fileUrls, int droppedIndex)
{
    QList<PlayListItem> items;
    for(QUrl &fileUrl:fileUrls) {
        std::filesystem::path path(fileUrl.path().toStdString());
        addFileOrFolderToPlayList(path, droppedIndex);
        //PlayListItem item = createPlayListItemObject(path, droppedIndex);
        //items.append(item);
    }
    ui->playListWidget->updateItemNumbers();
    //ui->playListWidget->addPlayListItems(items, droppedIndex);
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
    //setGeometry(windowGeometry);
}

void PlayListEditorWindow::hideEvent(QHideEvent * event)
{
    //windowGeometry = geometry();
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
