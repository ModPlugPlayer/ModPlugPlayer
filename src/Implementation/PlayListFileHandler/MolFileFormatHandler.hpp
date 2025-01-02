/*
MolFileFormatHandler class declarations of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <QObject>
#include "PlayListDTOs.hpp"
#include <filesystem>
#include <Interfaces/PlayListFileFormatHandler.hpp>

// first column represents module format, 0x24->MOD, if the format is not present, then first row is 0x20 (space)
// the second column represents the folder id, starts from 0x20

/*
    Open file with Xcode and press Command + Shift + J
    Right click file name in left pane
    Open as -> Hex
*/

#define BeginningDirectoryIndex 0x20

enum class MOLModuleFormat : char {
    _669 = 0x27,//<--filename field doesn't have ".extension" at the end
    IT = 0x33,//<----filename field doesn't have ".extension" at the end
    MOD = 0x24,//<---filename field doesn't have ".extension" at the end
    MTM = 0x2A,//<---filename field doesn't have ".extension" at the end
    S3M = 0x2D,//<---filename field doesn't have ".extension" at the end
    XM = 0x2F,//<----filename field doesn't have ".extension" at the end
    Other = 0x20,//<--filename has ".extension" at the end
    Unknown = 0x7F//<--not handled, take extension from the filename as like "Other"
};

enum class MOLSection {
    Directories,
    Archives,
    ArchiveDiretories,
    Files,
    Undefined
};

namespace ModPlugPlayer::Interfaces {
class MolFileFormatHandler : public PlayListFileFormatHandler {
public:
    std::vector<PlayListItem> loadPlayListFromFile(const std::filesystem::path &path) override;
    void savePlayListToFile(const std::vector<PlayListItem> &playListItems, const std::filesystem::path &path) override;
};
}
