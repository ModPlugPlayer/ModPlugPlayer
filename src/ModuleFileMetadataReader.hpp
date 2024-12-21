#pragma once
#include <filesystem>
#include <libopenmpt/libopenmpt.hpp>
#include <APIStructures.hpp>
class ModuleFileMetadataReader
{
public:
    ModuleFileMetadataReader(const std::filesystem::path &filePath);
    ~ModuleFileMetadataReader();
    ModPlugPlayer::ModuleFileInfo getModuleFileInfo();
private:
    openmpt::module *mod = nullptr;
    std::filesystem::path filePath;
};
