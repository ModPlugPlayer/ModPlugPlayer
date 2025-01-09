#include "ModuleFileMetadataReader.hpp"
#include <fstream>
#include <QDebug>
#include <MPPExceptions.hpp>
#include "Util/ModPlugPlayerUtil.hpp"

using namespace ModPlugPlayer;

ModPlugPlayer::SongFileInfo ModuleFileMetaDataReader::getMetaData(const std::filesystem::path &filePath) const {
    std::ifstream file(filePath, std::ios::binary);
    if(file.fail())
        throw ModPlugPlayer::Exceptions::FileNotFoundException(filePath);
    openmpt::module *mod = nullptr;
    try{
        mod = new openmpt::module(file);
    }
    catch(openmpt::exception &e) {
        qWarning() << "Error:" << e.what();
        if(mod != nullptr)
            delete mod;
        throw ModPlugPlayer::Exceptions::UnsupportedFileFormatException();
    }
    SongFileInfo songFileInfo = ModPlugPlayer::ModPlugPlayerUtil::createModuleFileInfoObject(mod, filePath);
    file.close();
    return songFileInfo;
}
