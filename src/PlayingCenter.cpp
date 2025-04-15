/*
PlayingCenter class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayingCenter.hpp"
#include "MessageCenter.hpp"
#include <Util/VectorUtil.hpp>
#include <VolumeControl.hpp>
#include <boost/algorithm/string.hpp>


PlayingCenter::PlayingCenter(QObject *parent)
    : QObject(parent) {
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));
    scrubberTimer = new QTimer(this);
    instantModuleInfoTimer = new QTimer(this);
    timeInfoTimer = new QTimer(this);
    scrubberTimer->setInterval(250);
    instantModuleInfoTimer->setInterval(100);
    timeInfoTimer->setInterval(100);
    connectSignalsAndSlots();
    portaudio::System::initialize();
}

PlayingCenter &PlayingCenter::getInstance() {
    static PlayingCenter instance;
    return instance;
}

double PlayingCenter::getVUMeterValue() {
    return moduleHandler.getVuMeterValue();
}

void PlayingCenter::getSpectrumData(double *spectrumData) {
    return moduleHandler.getSpectrumData(spectrumData);
}

PlayingState PlayingCenter::getPlayingState() {
    return moduleHandler.getPlayerState();
}

PlayingCenter::~PlayingCenter(){
    delete fileDialog;
    portaudio::System::terminate();
}

void PlayingCenter::connectSignalsAndSlots() {
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::openRequested), this, qOverload<>(&PlayingCenter::onOpenRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<const std::filesystem::path>(&MessageCenterRequests::SongRequests::openRequested), this, qOverload<const std::filesystem::path>(&PlayingCenter::onOpenRequested));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo, bool>(&MessageCenterEvents::SongEvents::loaded), this, qOverload<const SongFileInfo, bool>(&PlayingCenter::onLoaded));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem, bool>(&MessageCenterEvents::SongEvents::loaded), this, qOverload<const PlayListItem, bool>(&PlayingCenter::onLoaded));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::pauseRequested), this, qOverload<>(&PlayingCenter::onPauseRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::stopRequested), this, qOverload<>(&PlayingCenter::onStopRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::playRequested), this, qOverload<>(&PlayingCenter::onPlayRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<const PlayListItem>(&MessageCenterRequests::SongRequests::playRequested), this, qOverload<const PlayListItem>(&PlayingCenter::onPlayRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, &MessageCenterRequests::SongRequests::repeatModeChangeRequested, this, &PlayingCenter::onRepeatModeChangeRequested);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::volumeChangeRequested, this, &PlayingCenter::onVolumeChangeRequested);
    connect(&MessageCenter::getInstance().requests.scrubberRequests, &MessageCenterRequests::ScrubberRequests::scrubbingRequested, this, &PlayingCenter::onScrubbingRequested);
    connect(&MessageCenter::getInstance().requests.moduleRequests, &MessageCenterRequests::ModuleRequests::amigaFilterChangeRequested, this, &PlayingCenter::onAmigaFilterChangeRequested);
    connect(&MessageCenter::getInstance().requests.moduleRequests, &MessageCenterRequests::ModuleRequests::interpolationFilterChangeRequested, this, &PlayingCenter::onInterpolationFilterChangeRequested);

    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::outputDeviceChangeRequested, this, &PlayingCenter::onOutputDeviceChangeRequested);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::soundResolutionChangeRequested, this, &PlayingCenter::onSoundResolutionChangeRequested);

    connect(scrubberTimer, &QTimer::timeout, this, &PlayingCenter::updateScrubber);
    connect(instantModuleInfoTimer, &QTimer::timeout, this, &PlayingCenter::updateInstantModuleInfo);
    connect(timeInfoTimer, &QTimer::timeout, this, &PlayingCenter::updateTimeInfo);

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<>(&PlayingCenter::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<const SongFileInfo>(&PlayingCenter::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<const PlayListItem>(&PlayingCenter::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::stopped),this, qOverload<>(&PlayingCenter::onStopped));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::paused), this, qOverload<>(&PlayingCenter::onPaused));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::paused), this, qOverload<const SongFileInfo>(&PlayingCenter::onPaused));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::paused), this, qOverload<const PlayListItem>(&PlayingCenter::onPaused));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<>(&PlayingCenter::onResumed));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<const SongFileInfo>(&PlayingCenter::onResumed));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<const PlayListItem>(&PlayingCenter::onResumed));

}

void PlayingCenter::onVolumeChangeRequested(const int value) {
    double linearVolume = ((double)value)/100.0f;
    double exponentialVolume = DSP::VolumeControl<double>::calculateExponetialVolume(linearVolume);
    moduleHandler.setVolume(exponentialVolume);
    qDebug()<<"Requested linear Volume is"<<linearVolume;
    qDebug()<<"Volume is set to"<<exponentialVolume<<"as exponantial volume";
}

void PlayingCenter::onScrubbingRequested(const int position) {
    moduleHandler.scrubTime(position);
}

void PlayingCenter::onScrubberUpdatePeriodChangeRequested(const unsigned int updatePeriod) {
    scrubberTimer->setInterval(updatePeriod);
}

void PlayingCenter::updateInstantModuleInfo(){
    if(moduleHandler.getPlayerState() == PlayingState::Playing) {
        MessageCenter &messageCenter = MessageCenter::getInstance();
        currentActiveChannelAmount = moduleHandler.getActiveChannelAmount();
        currentSubSongIndex = moduleHandler.getCurrentSubSongIndex();
        currentPatternAmount = moduleHandler.getPatternAmount();
        currentPatternIndex = moduleHandler.getCurrentPatternIndex();
        if(currentActiveChannelAmount != previousActiveChannelAmount) {
            emit messageCenter.events.moduleEvents.activeChannelAmountChanged(currentActiveChannelAmount);
            previousActiveChannelAmount = currentActiveChannelAmount;
        }
        if(currentSubSongIndex != previousSubSongIndex) {
            emit messageCenter.events.moduleEvents.currentSubSongIndexChanged(currentSubSongIndex);
            previousSubSongIndex = currentSubSongIndex;
        }
        if(currentPatternAmount != previousPatternAmount) {
            emit messageCenter.events.moduleEvents.patternAmountChanged(currentPatternAmount);
            previousPatternAmount = currentPatternAmount;
        }
        if(currentPatternIndex != previousPatternIndex) {
            emit messageCenter.events.moduleEvents.currentPatternIndexChanged(currentPatternIndex);
            previousPatternIndex = currentPatternIndex;
        }
    }
}

void PlayingCenter::updateTimeInfo() {
    currentElapsedSeconds = moduleHandler.getTimeInfo().seconds;
    if(currentElapsedSeconds != previousElapsedSeconds) {
        emit MessageCenter::getInstance().events.songEvents.elapsedTimeChanged(currentElapsedSeconds);
        previousElapsedSeconds = currentElapsedSeconds;
    }
}

void PlayingCenter::onOpenRequested() {
    moduleHandler.stop();
    QString filePath;

    filePath = fileDialog->getOpenFileName(nullptr, "Open Module File",
                                           QString(), tr("All Modules") + " (" + getSupportedExtensionsAsString() + ")"
                                               + " ;; " + tr("Module Lists") + " (*.mol)"
                                               + " ;; " + tr("Compressed Modules") + " (*.mdz *.s3z *.xmz *.itz)"
                                               + " ;; " + tr("ProTracker Modules") + " (*.mod *.nst mod.* nst.*)"
                                               + " ;; " + tr("ScreamTracker Modules") + " (*.s3m *.stm)"
                                               + " ;; " + tr("FastTracker Modules") + " (*.xm)"
                                               + " ;; " + tr("ImpulseTracker Modules") + " (*.it)"
                                               + " ;; " + tr("Other Modules") + " (" + getLessKnownSupportedExtensionsAsString() + ")"
                                               + " ;; " + tr("Wave Files") + " (*.wav)"
                                               + " ;; " + tr("All Files") + " (*.*)"
                                           );
    if (!filePath.isEmpty()){
        std::filesystem::path path(filePath.toStdString());
        emit MessageCenter::getInstance().requests.songRequests.openRequested(path);
    }
}

void PlayingCenter::onOpenRequested(const std::filesystem::path filePath) {
    qDebug()<<"Open requested:"<< filePath;
    moduleHandler.load(filePath);
}

void PlayingCenter::onStopRequested() {
    //    if(playerState != PLAYERSTATE::STOPPED)
    moduleHandler.stop();
    emit MessageCenter::getInstance().events.songEvents.timeScrubbed(0);
}

void PlayingCenter::onStopRequested(const SongFileInfo songFileInfo) {
    onStopRequested();
}

void PlayingCenter::onStopRequested(const PlayListItem playListItem) {
    onStopRequested();
}

void PlayingCenter::onPlayRequested() {
    //    if(playerState != PLAYERSTATE::STOPPED)
    moduleHandler.play();
    //emit MessageCenter::getInstance().events.songEvents.playingStarted();
    qDebug()<<"Play";
}

void PlayingCenter::onPlayRequested(const SongFileInfo songFileInfo) {
    onPlayRequested();
    qDebug()<<"Play";
}

void PlayingCenter::onPlayRequested(PlayListItem playListItem) {
    moduleHandler.load(playListItem);
    onPlayRequested();
}

void PlayingCenter::onPauseRequested()
{
    moduleHandler.pause();
    qDebug()<<"Pause";
}

void PlayingCenter::onPauseRequested(const SongFileInfo songFileInfo) {
    onPauseRequested();
}

void PlayingCenter::onPauseRequested(PlayListItem playListItem) {
    onPauseRequested();
}

void PlayingCenter::onResumeRequested() {
    moduleHandler.resume();
}

void PlayingCenter::onResumeRequested(const SongFileInfo songFileInfo) {
    onResumeRequested();
}

void PlayingCenter::onResumeRequested(PlayListItem playListItem) {
    onResumeRequested();
}

void PlayingCenter::onRewindRequested() {

}

void PlayingCenter::onFastForwardRequested() {

}

void PlayingCenter::onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) {
    moduleHandler.setRepeatMode(repeatMode);
    emit MessageCenter::getInstance().events.songEvents.repeatModeChanged(repeatMode);
}

void PlayingCenter::onEqStateChangeRequested(const bool activated) {
    qInfo() << "Equalizer state was set to" << activated;
    emit MessageCenter::getInstance().events.soundEvents.eqStateChanged(activated);
}

void PlayingCenter::onDSPStateChangeRequested(const bool activated) {
    qInfo() << "DSP state was set to" << activated;
    emit MessageCenter::getInstance().events.soundEvents.dspStateChanged(activated);
}

void PlayingCenter::onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) {
    moduleHandler.setAmigaFilter(amigaFilter);
    qInfo()<<"Amiga filter changed to" << (int) amigaFilter;
}

void PlayingCenter::onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter)
{
    moduleHandler.setInterpolationFilter(interpolationFilter);
    qInfo()<<"Interpolation filter changed to" << (int) interpolationFilter;
}

void PlayingCenter::onSpectrumAnalyzerWindowFunctionChanged(const WindowFunction windowFunction) {
    moduleHandler.setSpectrumAnalyzerWindowFunction(windowFunction);
}

void PlayingCenter::onOutputDeviceChangeRequested(const int outputDeviceIndex){
    moduleHandler.setOutputDeviceIndex(outputDeviceIndex);
}

void PlayingCenter::onSoundResolutionChangeRequested(const SoundResolution soundResolution) {
    moduleHandler.setSoundResolution(soundResolution);
}

void PlayingCenter::repeatModeChangeRequested(const RepeatMode repeatMode) {
    emit MessageCenter::getInstance().events.songEvents.repeatModeChanged(repeatMode);
}

void PlayingCenter::onLoaded(const SongFileInfo songFileInfo, const bool successfull) {
    if(!successfull) {
        emit MessageCenter::getInstance().requests.songRequests.stopRequested();
        return; // To-do: warn user that the file can't be loaded
    }
    playingMode = PlayingMode::Song;
    currentSongFileInfo = songFileInfo;
    currentPlayListItem = PlayListItem();
    afterLoaded(songFileInfo);
}

void PlayingCenter::onLoaded(PlayListItem playListItem, bool successfull) {
    if(!successfull) {
        return; // To-do: warn user that the file can't be loaded
    }
    //emit MessageCenter::getInstance().requests.songRequests.stopRequested();
    playingMode = PlayingMode::PlayList;
    currentPlayListItem = playListItem;
    currentSongFileInfo = SongFileInfo();
    afterLoaded(playListItem.songFileInfo);
}

void PlayingCenter::afterLoaded(const SongFileInfo fileInfo) {
    std::string songTitle = moduleHandler.getSongTitle();
    QString title = QString::fromUtf8(songTitle);
    unsigned int globalRowAmount = moduleHandler.getGlobalRowAmount();
    if(title.trimmed().isEmpty())
        title = QString::fromStdString(moduleHandler.getFilePath().stem().string());
    emit MessageCenter::getInstance().events.songEvents.songTitleChanged(title.toStdString());
    emit MessageCenter::getInstance().events.scrubberEvents.scrubberStepsAmountChanged(globalRowAmount);

    emit MessageCenter::getInstance().events.songEvents.songDurationChanged(fileInfo.songInfo.songDuration);
    emit MessageCenter::getInstance().events.moduleEvents.moduleFormatChanged(boost::algorithm::to_upper_copy(fileInfo.songInfo.songFormat));
    emit MessageCenter::getInstance().events.moduleEvents.channelAmountChanged(moduleHandler.getChannelAmount());
    emit MessageCenter::getInstance().events.moduleEvents.activeChannelAmountChanged(moduleHandler.getActiveChannelAmount());
    emit MessageCenter::getInstance().events.moduleEvents.subSongAmountChanged(moduleHandler.getSubSongAmount());
    emit MessageCenter::getInstance().events.moduleEvents.currentSubSongIndexChanged(moduleHandler.getCurrentSubSongIndex());
    emit MessageCenter::getInstance().events.moduleEvents.patternAmountChanged(moduleHandler.getPatternAmount());
    emit MessageCenter::getInstance().events.moduleEvents.currentPatternIndexChanged(moduleHandler.getCurrentPatternIndex());
    emit MessageCenter::getInstance().requests.songRequests.playRequested();
}

void PlayingCenter::onPlayingStarted() {
    scrubberTimer->start();
    instantModuleInfoTimer->start();
    timeInfoTimer->start();
}

void PlayingCenter::onPlayingStarted(const SongFileInfo songFileInfo) {
}

void PlayingCenter::onPlayingStarted(const PlayListItem playListItem) {
}

void PlayingCenter::onStopped() {
    scrubberTimer->stop();
    instantModuleInfoTimer->stop();
    timeInfoTimer->stop();
}

void PlayingCenter::onStopped(const SongFileInfo songFileInfo) {
}

void PlayingCenter::onStopped(const PlayListItem playListItem) {
}

void PlayingCenter::onPaused() {
    scrubberTimer->stop();
    instantModuleInfoTimer->stop();
    timeInfoTimer->stop();
}

void PlayingCenter::onPaused(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onPaused(const PlayListItem playListItem) {

}

void PlayingCenter::onResumed() {
    scrubberTimer->start();
    instantModuleInfoTimer->start();
    timeInfoTimer->start();
}

void PlayingCenter::onResumed(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onResumed(const PlayListItem playListItem) {

}

void PlayingCenter::onRepeatModeChanged(const RepeatMode repeatMode) {
}

void PlayingCenter::updateScrubber() {
    currentScrubberPosition = moduleHandler.getCurrentRowGlobalIndex();
    if(currentScrubberPosition != previousScrubberPosition) {
        emit MessageCenter::getInstance().events.scrubberEvents.scrubberPositionChanged(currentScrubberPosition);
        previousScrubberPosition = currentScrubberPosition;
    }
}

QString PlayingCenter::getSupportedExtensionsAsString() {
    std::vector<std::string> supportedExtensions = moduleHandler.getSupportedExtensions();
    QString supportedExtensionListString;
    for(std::string &supportedExtension : supportedExtensions) {
        supportedExtensionListString += QString::fromStdString("*." + supportedExtension) + " ";
    }
    supportedExtensionListString +=  "mod.*  nst.*";
    return supportedExtensionListString;
}

QString PlayingCenter::getLessKnownSupportedExtensionsAsString()
{
    std::vector<std::string> lessKnownExtensions = moduleHandler.getSupportedExtensions();

    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "mod");
    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "nst");
    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "s3m");
    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "stm");
    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "xm");
    VectorUtil::eraseElementFromVector<std::string>(lessKnownExtensions, "it");

    QString lessKnownExtensionListString;
    for(std::string &lessKnownExtension : lessKnownExtensions) {
        lessKnownExtensionListString += QString::fromStdString("*." + lessKnownExtension) + " ";
    }

    return lessKnownExtensionListString.trimmed();
}


/**
    TimeInfo timeInfo = moduleHandler.getTimeInfo();
    emit MessageCenter::getInstance().events.songEvents.elapsedTimeChanged(moduleHandler.getTimeInfo().seconds);
    updateInstantModuleInfo();
    moduleHandler.scrubTime(scrubberClickedPosition);

*/
