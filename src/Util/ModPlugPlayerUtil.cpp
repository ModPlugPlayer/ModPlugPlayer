/*
ModPlugPlayerUtil namespace definitions of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ModPlugPlayerUtil.hpp"
#include <boost/uuid/uuid_generators.hpp>

size_t ModPlugPlayer::ModPlugPlayerUtil::getSongDuration(openmpt::module *module) {
    return module->get_duration_seconds();
}

std::vector<std::string> ModPlugPlayer::ModPlugPlayerUtil::getSupportedExtensions() {
    return openmpt::get_supported_extensions();
}

ModPlugPlayer::ModuleFileInfo ModPlugPlayer::ModPlugPlayerUtil::createModuleFileInfoObject(openmpt::module *module, std::filesystem::path moduleFilePath) {
    ModuleFileInfo moduleFileInfo;
    moduleFileInfo.id = boost::uuids::random_generator()();
    moduleFileInfo.filePath = moduleFilePath;
    moduleFileInfo.moduleInfo = getModuleInfo(module);
    return moduleFileInfo;
}

ModPlugPlayer::ModuleInfo ModPlugPlayer::ModPlugPlayerUtil::getModuleInfo(openmpt::module *module) {
    ModuleInfo moduleInfo;
    moduleInfo.moduleFormat = ModPlugPlayerUtil::MetaData::getModuleFormat(module);
    moduleInfo.moduleFormatName = ModPlugPlayerUtil::MetaData::getModuleFormatName(module);
    moduleInfo.artist = ModPlugPlayerUtil::MetaData::getArtist(module);
    moduleInfo.songDuration = ModPlugPlayerUtil::getSongDuration(module);
    moduleInfo.songTitle = ModPlugPlayerUtil::MetaData::getModuleTitle(module);
    return moduleInfo;
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleFormat(openmpt::module *module)
{
    return module->get_metadata("type");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleFormatName(openmpt::module *module)
{
    return module->get_metadata("type_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleOriginalFormat(openmpt::module *module)
{
    return module->get_metadata("originaltype");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleOriginalFormatName(openmpt::module *module) {
    return module->get_metadata("originaltype_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getContainerFormat(openmpt::module *module) {
    return module->get_metadata("container");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getContainerFormatName(openmpt::module *module) {
    return module->get_metadata("container_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getTrackerName(openmpt::module *module) {
    return module->get_metadata("tracker");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getArtist(openmpt::module *module) {
    return module->get_metadata("artist");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleTitle(openmpt::module *module) {
    return module->get_metadata("title");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getLastSavedDate(openmpt::module *module) {
    return module->get_metadata("date");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getSongMessage(openmpt::module *module) {
    return module->get_metadata("message");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getSongMessageRaw(openmpt::module *module) {
    return module->get_metadata("message_raw");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getWarnings(openmpt::module *module)
{
    return module->get_metadata("warnings");
}
