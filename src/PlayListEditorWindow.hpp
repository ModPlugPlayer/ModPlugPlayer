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
#include <Interfaces/Player.hpp>
#include "ModuleFileMetadataReader.hpp"

QT_BEGIN_NAMESPACE
    namespace Ui { class PlayListEditorWindow; }
QT_END_NAMESPACE

using namespace ModPlugPlayer;
using namespace ModPlugPlayer::Interfaces;

class PlayListEditorWindow : public QMainWindow {
    Q_OBJECT

    public:
     PlayListEditorWindow(QWidget *parent, Player *playerWindow);
    ~PlayListEditorWindow();
    signals:
        void hidden();
    public slots:
        void onFileDropped(QUrl fileUrl, int droppedIndex);
        void onFilesDropped(QList<QUrl> fileUrls, int droppedIndex);
        void onPlayPrevious();
        void onPlayNext();
    private slots:
        void on_Add_clicked();
        void on_Remove_clicked();
        void onVerticalScrollBarVisibilityChanged(bool visible);

    private:
        Ui::PlayListEditorWindow *ui;
        void closeEvent(QCloseEvent *event);
        void showEvent(QShowEvent *event);
        void hideEvent(QHideEvent *event);
        Player *playerWindow;
        void connectSignalsAndSlots();
        void addFileOrFolderToPlayList(const std::filesystem::path &path, int &droppedIndex);
};
