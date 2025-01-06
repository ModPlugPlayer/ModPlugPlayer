/*
ModPlugPlayerUtil namespace definitions of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ModPlugPlayerUtil.hpp"
#include <boost/uuid/uuid_generators.hpp>
#include <libopenmpt/libopenmpt.h>

size_t ModPlugPlayer::ModPlugPlayerUtil::getSongDuration(const openmpt::module *module) {
    return module->get_duration_seconds();
}

std::vector<std::string> ModPlugPlayer::ModPlugPlayerUtil::getSupportedExtensions() {
    return openmpt::get_supported_extensions();
}

ModPlugPlayer::ModuleFileInfo ModPlugPlayer::ModPlugPlayerUtil::createModuleFileInfoObject(const openmpt::module *module, const std::filesystem::path moduleFilePath) {
    ModuleFileInfo moduleFileInfo;
    moduleFileInfo.filePath = moduleFilePath;
    moduleFileInfo.moduleInfo = getModuleInfo(module);
    return moduleFileInfo;
}

ModPlugPlayer::ModuleInfo ModPlugPlayer::ModPlugPlayerUtil::getModuleInfo(const openmpt::module *module) {
    ModuleInfo moduleInfo;
    moduleInfo.moduleFormat = ModPlugPlayerUtil::MetaData::getModuleFormat(module);
    moduleInfo.moduleFormatName = ModPlugPlayerUtil::MetaData::getModuleFormatName(module);
    moduleInfo.artist = ModPlugPlayerUtil::MetaData::getArtist(module);
    moduleInfo.songDuration = ModPlugPlayerUtil::getSongDuration(module);
    moduleInfo.songTitle = ModPlugPlayerUtil::MetaData::getModuleTitle(module);
    return moduleInfo;
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleFormat(const openmpt::module *module)
{
    return module->get_metadata("type");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleFormatName(const openmpt::module *module)
{
    return module->get_metadata("type_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleOriginalFormat(const openmpt::module *module)
{
    return module->get_metadata("originaltype");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleOriginalFormatName(const openmpt::module *module) {
    return module->get_metadata("originaltype_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getContainerFormat(const openmpt::module *module) {
    return module->get_metadata("container");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getContainerFormatName(const openmpt::module *module) {
    return module->get_metadata("container_long");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getTrackerName(const openmpt::module *module) {
    return module->get_metadata("tracker");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getArtist(const openmpt::module *module) {
    return module->get_metadata("artist");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getModuleTitle(const openmpt::module *module) {
    return module->get_metadata("title");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getLastSavedDate(const openmpt::module *module) {
    return module->get_metadata("date");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getSongMessage(const openmpt::module *module) {
    return module->get_metadata("message");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getSongMessageRaw(const openmpt::module *module) {
    return module->get_metadata("message_raw");
}

std::string ModPlugPlayer::ModPlugPlayerUtil::MetaData::getWarnings(const openmpt::module *module) {
    return module->get_metadata("warnings");
}

void ModPlugPlayer::ModPlugPlayerUtil::Catalog::setInterpolationFilter(openmpt::module *module, const InterpolationFilter interpolationFilter) {
    module->set_render_param(OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, (std::int32_t) interpolationFilter);
}

void ModPlugPlayer::ModPlugPlayerUtil::Catalog::setAmigaEmulationType(openmpt::module *module, const AmigaFilter amigaFilter) {
    module->ctl_set_boolean("render.resampler.emulate_amiga", amigaFilter != AmigaFilter::DisablePaulaEmulation);
    if(amigaFilter != AmigaFilter::DisablePaulaEmulation)
        module->ctl_set_text("render.resampler.emulate_amiga_type", getAmigaFilterValue(amigaFilter));
}

void ModPlugPlayer::ModPlugPlayerUtil::Catalog::setSongEndBehavior(openmpt::module *module, const SongEndBehavior songEndBehavior) {
    std::string songEndBehaviorString;
    switch(songEndBehavior) {
        case ModPlugPlayer::ModPlugPlayerUtil::Catalog::SongEndBehavior::Stop:
            songEndBehaviorString = "stop";
            break;
        case ModPlugPlayer::ModPlugPlayerUtil::Catalog::SongEndBehavior::FadeOut:
            songEndBehaviorString = "fadeout";
            break;
        case ModPlugPlayer::ModPlugPlayerUtil::Catalog::SongEndBehavior::Continue:
            songEndBehaviorString = "continue";
            break;
    }
    module->ctl_set_text("play.at_end", songEndBehaviorString);
}

void ModPlugPlayer::ModPlugPlayerUtil::setRepeatMode(openmpt::module *module, const RepeatMode repeatMode) {
    int repeatCount = 0;
    switch(repeatMode) {
        case RepeatMode::NoRepeat:
        case RepeatMode::RepeatPlayList:
        case RepeatMode::RepeatTrack:
            repeatCount = 0;
            break;
        case RepeatMode::LoopTrack:
            repeatCount = -1;
    }

    module->set_repeat_count((std::int32_t) repeatCount);
}

std::string ModPlugPlayer::ModPlugPlayerUtil::getAmigaFilterValue(const AmigaFilter amigaFilter) {
    std::string amigaFilterValue;
    switch(amigaFilter) {
    case AmigaFilter::Auto:
        amigaFilterValue = "auto";
        break;
    case AmigaFilter::Amiga500:
        amigaFilterValue = "a500";
        break;
    case AmigaFilter::Amiga1200:
        amigaFilterValue = "a1200";
        break;
    case AmigaFilter::Unfiltered:
        amigaFilterValue = "unfiltered";
        break;
    case AmigaFilter::DisablePaulaEmulation:
        amigaFilterValue = "";
        break;
    }
    return amigaFilterValue;
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getChannelAmount(const openmpt::module *module) {
    return module->get_num_channels();
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getActiveChannelAmount(const openmpt::module *module)
{
    return module->get_current_playing_channels();
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getSubSongAmount(const openmpt::module *module) {
    return module->get_num_subsongs();
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getCurrentSubSongIndex(const openmpt::module *module) {
    return module->ctl_get_integer("subsong");
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getPatternAmount(const openmpt::module *module) {
    return module->get_num_patterns();
}

size_t ModPlugPlayer::ModPlugPlayerUtil::getCurrentPatternIndex(const openmpt::module *module) {
    return module->get_current_pattern();
}

ModPlugPlayer::ModuleFileInfo ModPlugPlayer::ModPlugPlayerUtil::createCorruptedModuleFileInfoObject(const std::filesystem::path moduleFilePath) {
    ModuleFileInfo moduleFileInfo;
    moduleFileInfo.filePath = moduleFilePath;
    moduleFileInfo.successful = false;
    return moduleFileInfo;
}
