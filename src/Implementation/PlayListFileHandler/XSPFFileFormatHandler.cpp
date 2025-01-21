/*
XSPFFileFormatHandler class definitions of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "XSPFFileFormatHandler.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <Util/FileUtil.hpp>
#include <QDebug>

using namespace ModPlugPlayer;
using namespace boost;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

template<class Ptree>
inline const Ptree & empty_ptree() {
    static Ptree pt;
    return pt;
}

std::vector<PlayListItem> ModPlugPlayer::XSPFFileFormatHandler::loadPlayListFromFile(const std::filesystem::path &path) {
    std::vector<PlayListItem> playListItems;

    ptree root;
    read_xml(path.string(), root);

    std::string title = root.get<std::string>("playlist.title");
    auto trackList = root.get_child("playlist.trackList");

    int i = 0;
    for (const ptree::value_type &iterator: trackList) {
        PlayListItem playListItem;
        const ptree &track = iterator.second;

        std::string fileURI = track.get<std::string>("location","");
        if(fileURI == "")
            continue;
        if(fileURI.starts_with("file://"))
            playListItem.songFileInfo.filePath = FileUtil::fileURI2FilePath(fileURI);
        else
            playListItem.songFileInfo.filePath = fileURI;
        std::string duration = track.get<std::string>("duration", "0");
        playListItem.songFileInfo.songInfo.songDuration = std::stoi(duration)/1000;

        playListItem.itemNumber = i++;
        playListItem.dirty = true;
        playListItems.push_back(playListItem);
    }
    return playListItems;
}

void ModPlugPlayer::XSPFFileFormatHandler::savePlayListToFile(const std::vector<PlayListItem> &playListItems, const std::filesystem::path &path) {
    ptree root;
    ptree playlist;
    playlist.put("title", "Playlist");

    ptree playlistAttributes;

    ptree trackList;

    playlistAttributes.put("xmlns","http://xspf.org/ns/0");
    playlistAttributes.put("version","1");
    playlist.add_child("<xmlattr>",playlistAttributes);

    for(const PlayListItem &playListItem : playListItems) {
        ptree track;
        track.put("title", playListItem.songFileInfo.songInfo.songTitle);
        track.put("location", FileUtil::filePath2FileURI(playListItem.songFileInfo.filePath.string()));
        track.put("duration", std::to_string(playListItem.songFileInfo.songInfo.songDuration*1000));
        trackList.add_child("track", track);
    }

    playlist.add_child("trackList", trackList);
    root.add_child("playlist",playlist);
    write_xml(path.string(), root, std::locale(), xml_writer_make_settings<std::string>(' ', 1));
}
