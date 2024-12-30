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
#include <boost/url.hpp>
#include <QDebug>

using namespace ModPlugPlayer;
using namespace boost;
using namespace boost::property_tree;
using namespace boost::urls;

template<class Ptree>
inline const Ptree & empty_ptree() {
    static Ptree pt;
    return pt;
}

std::vector<PlayListItem> ModPlugPlayer::Interfaces::XSPFFileFormatHandler::loadPlayListFromFile(const std::filesystem::path &path) {
    std::vector<PlayListItem> playListItems;

    ptree root;
    read_xml(path, root);

    std::string title = root.get<std::string>("playlist.title");
    auto trackList = root.get_child("playlist.trackList");

    int i = 0;
    //BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, trackList) {
    for (auto& iterator: trackList) {
        PlayListItem playListItem;
        ptree &track = iterator.second;

        std::string fileURI = track.get<std::string>("location","");
        if(fileURI == "")
            continue;
        boost::core::string_view s = fileURI;
        boost::system::result<url_view> r = parse_uri(s);
        url_view u = r.value();

        playListItem.filePath = u.path();

        std::string duration = track.get<std::string>("duration", "0");
        if(duration == "0")
            continue;
        playListItem.duration = std::stoi(duration)/1000;

        playListItem.itemNumber = i++;
        playListItems.push_back(playListItem);
    }
    return playListItems;
}

void ModPlugPlayer::Interfaces::XSPFFileFormatHandler::savePlayListToFile(const std::vector<PlayListItem> &playListItems, const std::filesystem::path &path) {
    boost::property_tree::ptree pt;
}
