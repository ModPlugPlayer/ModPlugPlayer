/*
ModPlugPlayerUtil namespace declarations of ModPlug Player
Copyright (C) 2024 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <libopenmpt/libopenmpt.hpp>
#include <vector>
#include <string>
#include <APIStructures.hpp>

namespace  ModPlugPlayer {
    namespace ModPlugPlayerUtil {
        size_t getSongDuration(openmpt::module *module);
        std::vector<std::string> getSupportedExtensions();
        ModuleInfo getModuleInfo(openmpt::module *module);
        ModuleFileInfo createModuleFileInfoObject(openmpt::module *module, std::filesystem::path moduleFilePath);
        namespace MetaData{
            // Gets module format extension (e.g. it) or another similar identifier for modules formats that typically do not use a file extension
            std::string getModuleFormat(openmpt::module *module);

            // Gets format name associated with the module format (e.g. Impulse Tracker)
            std::string getModuleFormatName(openmpt::module *module);

            // Gets module format extension (e.g. it) of the original module in case the actual type is a converted format (e.g. mo3 or gdm)
            std::string getModuleOriginalFormat(openmpt::module *module);

            // Gets format name associated with the module format (e.g. Impulse Tracker) of the original module in case the actual type is a converted format (e.g. mo3 or gdm)
            std::string getModuleOriginalFormatName(openmpt::module *module);

            // Gets container format the module file is embedded in, if any (e.g. umx)
            std::string getContainerFormat(openmpt::module *module);

            // Gets full container name if the module is embedded in a container (e.g. Unreal Music)
            std::string getContainerFormatName(openmpt::module *module);

            // Gets tracker that was (most likely) used to save the module file, if known
            std::string getTrackerName(openmpt::module *module);

            // Gets author of the module
            std::string getArtist(openmpt::module *module);

            // Gets module title
            std::string getModuleTitle(openmpt::module *module);

            // Gets date the module was last saved, in ISO-8601 format.
            std::string getLastSavedDate(openmpt::module *module);

            // Gets song message. If the song message is empty or the module format does not support song messages, a list of instrument and sample names is returned instead.
            std::string getSongMessage(openmpt::module *module);

            // Gets song message. If the song message is empty or the module format does not support song messages, an empty string is returned.
            std::string getSongMessageRaw(openmpt::module *module);

            // Gets a list of warnings that were generated while loading the module.
            std::string getWarnings(openmpt::module *module);
        }

        namespace Catalog {
            enum class SongEndBehavior {
                Stop, FadeOut, Continue
            };

            void setInterpolationFilter(openmpt::module *module, InterpolationFilter interpolationFilter);
            void setAmigaEmulationType(openmpt::module *module, AmigaFilterType amigaFilterType);
            void setSongEndBehavior(openmpt::module *module, SongEndBehavior songEndBehavior);
        }
    };
};
