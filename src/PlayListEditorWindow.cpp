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
#include "ModuleFileMetadataReader.hpp"
#include "MPPExceptions.hpp"
#include "Implementation/PlayListFileHandler/XSPFFileFormatHandler.hpp"
#include "Implementation/PlayListFileHandler/ExtendedM3UFileFormatHandler.hpp"
#include "Implementation/PlayListFileHandler/MolFileFormatHandler.hpp"
#include <boost/algorithm/string.hpp>

PlayListEditorWindow::PlayListEditorWindow(QWidget *parent, Player *playerWindow)
    : QMainWindow(parent)
    , ui(new Ui::PlayListEditorWindow)
{
    ui->setupUi(this);
    this->playerWindow = playerWindow;
    connectSignalsAndSlots();
    ui->playListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    //fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));
    playListFileFormatHandler.XSPF = new XSPFFileFormatHandler();
    playListFileFormatHandler.extendedM3U = new ExtendedM3UFileFormatHandler();
    playListFileFormatHandler.MOL = new MolFileFormatHandler();
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
    connect(ui->playListWidget, &PlayListWidget::verticalScrollBarVisibilityChanged, this, &PlayListEditorWindow::onVerticalScrollBarVisibilityChanged);
    //connect(this, &PlayListEditorWindow::clearPlayList, ui->playListWidget, &PlayListWidget::clearPlayListRequested);
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
    delete (XSPFFileFormatHandler *) playListFileFormatHandler.XSPF;
    delete (ExtendedM3UFileFormatHandler *) playListFileFormatHandler.extendedM3U;
    delete (MolFileFormatHandler *) playListFileFormatHandler.MOL;
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
    ui->playListWidget->addPlayListItem(item);
    //ui->playListWidget->insertItem(ui->playListWidget->count(), QString::number(ui->playListWidget->count()));


    //ui->playListWidget->addItem(QString::number(ui->playListWidget->count()));

}

void PlayListEditorWindow::on_Remove_clicked()
{
    ui->playListWidget->removeSelectedItems();
}

void PlayListEditorWindow::onVerticalScrollBarVisibilityChanged(bool visible)
{
    int baseSize = 74;
    if(visible) {
        ui->playListHeader->setDurationLabelWidth(baseSize+10);
    }
    else {
        ui->playListHeader->setDurationLabelWidth(baseSize);
    }
}

void PlayListEditorWindow::on_LoadList_clicked()
{
    QString filePathString;

    filePathString = fileDialog->getOpenFileName(this, "Open Module File",
                                           QString(), tr("All PlayList Formats") + " (*.xspf *.m3u *.mol)"
                                               + " ;; " + tr("XML Shareable Playlist Format (XSPF") + " (*.xspf)"
                                               + " ;; " + tr("Extended M3U") + " (*.m3u)"
                                               + " ;; " + tr("Module Lists") + " (*.mol)"
                                               + " ;; " + tr("All Files") + " (*.*)"
                                           );
    if (!filePathString.isEmpty()){

        std::filesystem::path filePath(filePathString.toStdString());
        std::vector<PlayListItem> playListItems;
        std::string fileExtension = boost::algorithm::to_lower_copy(filePath.extension().string());
        if(fileExtension == ".xspf")
            playListItems = playListFileFormatHandler.XSPF->loadPlayListFromFile(filePath);
        else if(fileExtension == ".m3u")
            playListItems = playListFileFormatHandler.extendedM3U->loadPlayListFromFile(filePath);
        else if(fileExtension == ".mol")
            playListItems = playListFileFormatHandler.MOL->loadPlayListFromFile(filePath);
        ui->playListWidget->onClearPlayListRequested();
        for(PlayListItem playListItem : playListItems) {
            ui->playListWidget->addPlayListItem(playListItem);
        }
        ui->playListWidget->updateItemNumbers();
    }
}


void PlayListEditorWindow::on_SaveList_clicked()
{
    QString filePathString;

    filePathString = fileDialog->getSaveFileName(this, "Open Module File",
                                           QString(), tr("XML Shareable Playlist Format (XSPF") + " (*.xspf)"
                                           + " ;; " + tr("Extended M3U") + " (*.m3u)"
                                           + " ;; " + tr("Module Lists") + " (*.mol)"
                                           );
    if (!filePathString.isEmpty()){
        std::filesystem::path filePath(filePathString.toStdString());
        qDebug()<<filePath;
        std::vector<PlayListItem> playListItems = ui->playListWidget->getAllItems();
        std::string fileExtension = boost::algorithm::to_lower_copy(filePath.extension().string());
        if(fileExtension == ".xspf")
            playListFileFormatHandler.XSPF->savePlayListToFile(playListItems, filePath);
        else if(fileExtension == ".m3u")
            playListFileFormatHandler.extendedM3U->savePlayListToFile(playListItems, filePath);
        else if(fileExtension == ".mol")
            playListFileFormatHandler.MOL->savePlayListToFile(playListItems, filePath);
    }
}

