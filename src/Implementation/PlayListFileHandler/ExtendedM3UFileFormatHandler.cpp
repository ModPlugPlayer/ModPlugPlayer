/*
ExtendedM3UFileFormatHandler class definitions of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "ExtendedM3UFileFormatHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <Util/FileUtil.hpp>
#include <QDebug>

using namespace ModPlugPlayer;
using namespace std;

std::vector<PlayListItem> ModPlugPlayer::ExtendedM3UFileFormatHandler::loadPlayListFromFile(const std::filesystem::path &path) {
    std::vector<PlayListItem> playListItems;
    QFile inputFile(path);
    //Todo: replace the lines below by exceptions (e.g. FileNotFoundException).
    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file";
        qDebug() << inputFile.errorString();
        return playListItems;
    }
    else {
        qDebug() << "File opened";
    }

    qDebug() << "File exists: " << inputFile.exists();

    QTextStream inputStream(&inputFile);
    while (!inputStream.atEnd()) {
        QString line = inputStream.readLine().trimmed();
        if(line.startsWith("#"))
            continue;
        PlayListItem playListItem;
        if(line.startsWith("file://"))
            playListItem.filePath = FileUtil::fileURI2FilePath(line.toStdString());
        else
            playListItem.filePath = std::filesystem::path(line.toStdString());
        playListItem.dirty = true;
        playListItems.push_back(playListItem);
    }
    inputFile.close();
    return playListItems;
}

void ModPlugPlayer::ExtendedM3UFileFormatHandler::savePlayListToFile(const std::vector<PlayListItem> &playListItems, const std::filesystem::path &path) {
    QFile outputFile(path);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream outputStream(&outputFile);

    outputStream << "#EXTM3U" << Qt::endl;
    for(const PlayListItem &playListItem : playListItems) {
        outputStream << "#EXTINF:" << playListItem.duration << ","<<playListItem.title << Qt::endl;
        outputStream << FileUtil::filePath2FileURI(playListItem.filePath).c_str() <<Qt::endl;
    }


    outputStream.flush();
    outputFile.flush();
    outputFile.close();
}
