#pragma once
#include <filesystem>
#include <libopenmpt/libopenmpt.hpp>
#include <APIStructures.hpp>
#include <Interfaces/MetaDataReader.hpp>

class ModuleFileMetaDataReader : public ModPlugPlayer::Interfaces::MetaDataReader {
public:
    ModPlugPlayer::SongFileInfo getMetaData(const std::filesystem::path &filePath) const override;
};
