/*
PlayListEditorWindow class declarations for the test purposes of PlayListWidget
Copyright (C) 2022 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <QMainWindow>
#include <QUrl>
#include <QList>
#include <QFileDialog>
#include <Interfaces/Player.hpp>
#include <Interfaces/PlayListFileFormatHandler.hpp>
#include "ModuleFileMetadataReader.hpp"
#include <PlayListWidget.hpp>

using namespace ModPlugPlayer::Interfaces;

QT_BEGIN_NAMESPACE
    namespace Ui { class PlayListEditorWindow; }
QT_END_NAMESPACE

using namespace ModPlugPlayer;

class PlayListEditorWindow : public QMainWindow {
    Q_OBJECT

    public:
        PlayListEditorWindow(QWidget *parent = nullptr);
        PlayListWidget * getPlayListWidget();
        ~PlayListEditorWindow();
    signals:
        void hidden();
    public slots:
        void onFileDropped(QUrl fileUrl, int droppedIndex);
        void onFilesDropped(QList<QUrl> fileUrls, int droppedIndex);
        void onPreviousRequested();
        void onNextRequested();
    private slots:
        void on_Add_clicked();
        void on_Remove_clicked();
        void onVerticalScrollBarVisibilityChanged(bool visible);
        void on_LoadList_clicked();
        void on_SaveList_clicked();
        void onPlayListEditorWindowRequested(bool turnOn);

    private:
        Ui::PlayListEditorWindow *ui;
        void closeEvent(QCloseEvent *event);
        void showEvent(QShowEvent *event);
        void hideEvent(QHideEvent *event);
        void connectSignalsAndSlots();
        void addFileOrFolderToPlayList(const std::filesystem::path &path, int &droppedIndex);
        void updateDirtyItems();
        ModuleFileMetaDataReader metaDataReader;
        QFileDialog *fileDialog = nullptr;
        struct {
            PlayListFileFormatHandler *XSPF = nullptr;
            PlayListFileFormatHandler *extendedM3U = nullptr;
            PlayListFileFormatHandler *MOL = nullptr;
        } playListFileFormatHandler;
};
