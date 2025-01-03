#include "ModuleFileMetadataReader.hpp"
#include <fstream>
#include <QDebug>
#include <MPPExceptions.hpp>
#include "Util/ModPlugPlayerUtil.hpp"

using namespace ModPlugPlayer;

ModuleFileMetadataReader::ModuleFileMetadataReader(const std::filesystem::path &filePath) {
    this->filePath = filePath;
    std::ifstream file(filePath, std::ios::binary);
    if(mod != nullptr)
        delete mod;
    try{
        mod = new openmpt::module(file);
    }
    catch(openmpt::exception &e) {
        qWarning() << "Error:" << e.what();
        throw ModPlugPlayer::Exceptions::UnsupportedFileFormatException();
    }
}

ModuleFileMetadataReader::~ModuleFileMetadataReader() {
    if(mod != nullptr)
        delete mod;
}

ModPlugPlayer::ModuleFileInfo ModuleFileMetadataReader::getModuleFileInfo() {
    ModuleFileInfo moduleFileInfo = ModPlugPlayer::ModPlugPlayerUtil::createModuleFileInfoObject(mod, filePath);
    return moduleFileInfo;
}
