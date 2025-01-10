/*
MolFileFormatHandler class definitions of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "MolFileFormatHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <Util/FileUtil.hpp>
#include <QDebug>
#include <string>
#include <Util/VectorUtil.hpp>
#include <boost/algorithm/string.hpp>

using namespace ModPlugPlayer;
using namespace std;

char getModuleFormatId(const std::string &extension) {
    string ext = boost::algorithm::to_lower_copy(extension);
    if(ext == ".669")
        return 0x27;
    if(ext == ".it")
        return 0x33;
    if(ext == ".mod")
        return 0x24;
    if(ext == ".mtm")
        return 0x2A;
    if(ext == ".s3m")
        return 0x2D;
    if(ext == ".xm")
        return 0x2F;
    return 0x20;
}

string getModuleFormatString(MOLModuleFormat moduleFormat) {
    if(moduleFormat == MOLModuleFormat::_669)
        return ".669";
    if(moduleFormat == MOLModuleFormat::IT)
        return ".it";
    if(moduleFormat == MOLModuleFormat::MOD)
        return ".mod";
    if(moduleFormat == MOLModuleFormat::MTM)
        return ".mtm";
    if(moduleFormat == MOLModuleFormat::S3M)
        return ".s3m";
    if(moduleFormat == MOLModuleFormat::XM)
        return ".xm";
    return "";

}

//Module format is taken from the first column of the row
MOLModuleFormat getMOLModuleFormat(string extension) {
    string ext = boost::algorithm::to_lower_copy(extension);
    if(ext == ".669")
        return MOLModuleFormat::_669;
    if(ext == ".it")
        return MOLModuleFormat::IT;
    if(ext == ".mod")
        return MOLModuleFormat::MOD;
    if(ext == ".mtm")
        return MOLModuleFormat::MTM;
    if(ext == ".s3m")
        return MOLModuleFormat::S3M;
    if(ext == ".xm")
        return MOLModuleFormat::XM;
    return MOLModuleFormat::Other;
}

MOLModuleFormat getMOLModuleFormat(char moduleFormatId) {
    switch(moduleFormatId) {
    case 0x27:
        return MOLModuleFormat::_669;
    case 0x33:
        return MOLModuleFormat::IT;
    case 0x24:
        return MOLModuleFormat::MOD;
    case 0x2A:
        return MOLModuleFormat::MTM;
    case 0x2D:
        return MOLModuleFormat::S3M;
    case 0x2F:
        return MOLModuleFormat::XM;
    case 0x20:
        return MOLModuleFormat::Other;
    default:
        return MOLModuleFormat::Unknown;
    }
    return MOLModuleFormat::Unknown;
}

//Folder index is taken from the second column of the row
int getFolderIndex(const char &folderIndexId) {
    return int(folderIndexId) - int(BeginningDirectoryIndex);
}

char getFolderIndexId(const int &folderIndex) {
    return char(folderIndex) + char(BeginningDirectoryIndex);
}

// First column of the row represents the file format, 0x24->MOD, if the format is not presented, then 0x20 space character is used
// Second column of the row represents the folder index id, starts from 0x20

std::vector<PlayListItem> ModPlugPlayer::MolFileFormatHandler::loadPlayListFromFile(const std::filesystem::path &path) {
    std::vector<PlayListItem> playListItems;
    std::vector<std::string> directories;
    MOLSection molSection = MOLSection::Undefined;
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
        QString line = inputStream.readLine();
        QString trimmedLine = line.trimmed();
        if(trimmedLine == "[DIRECTORIES]") {
            molSection = MOLSection::Directories;
            continue;
        }
        else if(trimmedLine == "[FILES]") {
            molSection = MOLSection::Files;
            continue;
        }
        else if(trimmedLine == "[ARCHIVES]") {
            molSection = MOLSection::Archives;
            continue;
        }
        else if(trimmedLine == "[ARCHIVEDIRECTORIES]") {
            molSection = MOLSection::ArchiveDiretories;
            continue;
        }
        else if(molSection == MOLSection::Directories) {
            if(!line.isEmpty())
                directories.push_back(line.right(line.size()-1).toStdString());
        }
        else if(molSection == MOLSection::Files) {
            if(line.size() < 5)
                continue;
            if(line[2].toLatin1() != 0x1e)
                continue;
            MOLModuleFormat moduleFormat = getMOLModuleFormat(line[0].toLatin1());
            int folderIndex = getFolderIndex(line[1].toLatin1());
            QString fileName = line.right(line.size() - 4);
            PlayListItem playListItem;
            playListItem.dirty = true;
            playListItem.filePath = std::filesystem::path(directories[folderIndex]);

            if(moduleFormat != MOLModuleFormat::Other || moduleFormat != MOLModuleFormat::Unknown)
               fileName += getModuleFormatString(moduleFormat);

            playListItem.filePath.append(fileName.toStdString());
            playListItems.push_back(playListItem);
        }
    }
    inputFile.close();
    return playListItems;
}

void ModPlugPlayer::MolFileFormatHandler::savePlayListToFile(const std::vector<PlayListItem> &playListItems, const std::filesystem::path &path) {
    std::vector<std::string> directories;

    QFile outputFile(path);
    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream outputStream(&outputFile);

    for(const PlayListItem &playListItem : playListItems) {
        string directory = playListItem.filePath.parent_path();
        if(!VectorUtil::elementExists<string>(directories, directory)){
            directories.push_back(directory);
        }
    }

    outputStream << "[DIRECTORIES]" << Qt::endl;

    for(const string & directory : directories) {
        outputStream << " " << directory.c_str() << Qt::endl;
    }

    outputStream << "[ARCHIVES]" << Qt::endl;
    outputStream << "[ARCHIVDIRECTORIES]" << Qt::endl;
    outputStream << "[FILES]" << Qt::endl;

    for(const PlayListItem &playListItem : playListItems) {
        string directory = playListItem.filePath.parent_path();
        string fileName = playListItem.filePath.filename();
        string fileNameWithoutExtension = playListItem.filePath.stem();
        string fileExtension = playListItem.filePath.extension();
        int folderIndex = VectorUtil::findIndexOfElement<string>(directories, directory);
        char folderIndexId = getFolderIndexId(folderIndex);
        char moduleFormatId = getModuleFormatId(fileExtension);
        MOLModuleFormat moduleFormat = getMOLModuleFormat(moduleFormatId);
        outputStream << moduleFormatId << folderIndexId << char(0x1e) << " ";
        if(moduleFormat == MOLModuleFormat::Other || moduleFormat == MOLModuleFormat::Unknown)
            outputStream << fileName.c_str();
        else
            outputStream << fileNameWithoutExtension.c_str();
        outputStream << Qt::endl;
    }


    outputStream.flush();
    outputFile.flush();
    outputFile.close();
}
