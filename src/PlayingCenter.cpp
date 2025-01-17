/*
PlayingCenter class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayingCenter.hpp"
#include "MessageCenter.hpp"

PlayingCenter::PlayingCenter(QObject *parent)
    : QObject(parent) {
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));
    connectSignalsAndSlots();
}

PlayingCenter::~PlayingCenter(){
    delete fileDialog;
}

void PlayingCenter::updateInstantModuleInfo(){
    if(moduleHandler.getPlayerState() == PlayerState::Playing) {
        emit activeChannelAmountChanged(moduleHandler.getActiveChannelAmount());
        emit currentSubSongIndexChanged(moduleHandler.getCurrentSubSongIndex());
        emit patternAmountChanged(moduleHandler.getPatternAmount());
        emit currentPatternIndexChanged(moduleHandler.getCurrentPatternIndex());
    }
}

void PlayingCenter::onOpenRequested() {
    moduleHandler.onStopRequested();
    QString filePath;

    filePath = fileDialog->getOpenFileName(this, "Open Module File",
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
        emit(MessageCenter::getInstance().openRequested(path));
    }
}

void PlayingCenter::onOpenRequested(const std::filesystem::path filePath) {
    qDebug()<<"Open requested:"<< filePath;
}

void PlayingCenter::onStopRequested()
{
    //    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->stop();
    moduleHandler.onStopRequested();
    emit MessageCenter::getInstance().timeScrubbed(0);
}

void PlayingCenter::onStopRequested(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onStopRequested(const PlayListItem playListItem) {

}

void PlayingCenter::onPlayRequested() {
    //    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->start(spectrumAnalyzerTimerTimeoutValue);
    emit MessageCenter::getInstance().playingStarted();
    qDebug()<<"Play";
}

void PlayingCenter::onPlayRequested(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onPlayRequested(PlayListItem playListItem) {
    moduleHandler.load(playListItem);
    emit MessageCenter::getInstance().playingStarted(playListItem);
    onPlayRequested();
    qDebug()<< "onPlayingStarted" << playListItem.songFileInfo.songInfo.songTitle;
}

void PlayingCenter::onPauseRequested()
{
    //    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Pause";
}

void PlayingCenter::onPauseRequested(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onPauseRequested(PlayListItem playListItem) {

}

void PlayingCenter::onResumeRequested() {

}

void PlayingCenter::onResumeRequested(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onResumeRequested(PlayListItem playListItem) {

}


void PlayingCenter::onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) {
    WindowUtil::setAlwaysOnTop(this, alwaysOnTop);
    ui->actionAlways_On_Top->setChecked(alwaysOnTop);
    parameters->alwaysOnTop = alwaysOnTop;
}

void PlayingCenter::onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) {
    ui->actionSnap_to_Viewport->setChecked(snapToViewPort);
    moveByMouseClick->setSnapToViewPort(snapToViewPort);
    parameters->snapToViewPort = snapToViewPort;
}

void PlayingCenter::onSnappingThresholdChangeRequested(const int snappingThreshold) {
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
}

void PlayingCenter::onPreviousRequested() {
    qDebug()<<"Previous Requested";
    emit MessageCenter::getInstance().previousRequested();
}

void PlayingCenter::onPreviousRequested(const PlayListItem playListItem) {
    qDebug()<<"Previous Requested:"<<playListItem.songFileInfo.filePath;
    //emit MessageCenter::getInstance().previousRequested(playListItem);
}

void PlayingCenter::onNextRequested() {
    emit MessageCenter::getInstance().nextRequested();
    qDebug()<<"Next Requested";
}

void PlayingCenter::onNextRequested(const PlayListItem playListItem) {
    qDebug()<<"Next Requested:"<<playListItem.songFileInfo.filePath;
    //emit MessageCenter::getInstance().nextRequested(playListItem);
}

void PlayingCenter::onRewindRequested() {

}

void PlayingCenter::onFastForwardRequested() {

}

void PlayingCenter::onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) {
    moduleHandler.setRepeatMode(repeatMode);
    emit MessageCenter::getInstance().repeatModeChanged(repeatMode);
}

void PlayingCenter::onEqStateChangeRequested(const bool activated) {
    parameters->eqEnabled = activated;
    qInfo() << "Equalizer state was set to" << activated;
    emit MessageCenter::getInstance().eqStateChanged(activated);
}

void PlayingCenter::onDSPStateChangeRequested(const bool activated) {
    parameters->dspEnabled = activated;
    qInfo() << "DSP state was set to" << activated;
    emit MessageCenter::getInstance().dspStateChanged(activated);
}

void PlayingCenter::onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) {
    parameters->amigaFilter = amigaFilter;
    moduleHandler.setAmigaFilter(amigaFilter);
    qInfo()<<"Amiga filter changed to" << (int) amigaFilter;
    emit amigaFilterChanged(amigaFilter);
}



void PlayingCenter::onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter)
{
    parameters->interpolationFilter = interpolationFilter;
    moduleHandler.setInterpolationFilter(interpolationFilter);
    qInfo()<<"Interpolation filter changed to" << (int) interpolationFilter;
    emit interpolationFilterChanged(interpolationFilter);
}

void PlayingCenter::onSetupRequested() {
    parameters->save();
    bool stateAlwaysOnTop = isAlwaysOnTop();
    WindowUtil::setAlwaysOnTop(this, false);
    SetupWindow setupWindow(parameters, this);
    setupWindow.exec();
    WindowUtil::setAlwaysOnTop(this, stateAlwaysOnTop);
}

void PlayingCenter::onLoaded(const SongFileInfo songFileInfo, const bool successfull) {
    if(!successfull) {
        return; // To-do: warn user that the file can't be loaded
    }
    playingMode = PlayingMode::SingleTrack;
    currentSongFileInfo = songFileInfo;
    currentPlayListItem = PlayListItem();
    afterLoaded(songFileInfo);
}

void PlayingCenter::onLoaded(PlayListItem playListItem, bool successfull) {
    if(!successfull) {
        return; // To-do: warn user that the file can't be loaded
    }
    emit MessageCenter::getInstance().stopRequested();
    playingMode = PlayingMode::PlayList;
    currentPlayListItem = playListItem;
    currentSongFileInfo = SongFileInfo();
    afterLoaded(playListItem.songFileInfo);
}

void PlayingCenter::afterLoaded(const SongFileInfo fileInfo) {
    std::string songTitle = moduleHandler.getSongTitle();
    QString title = QString::fromUtf8(songTitle);
    if(title.trimmed().isEmpty())
        title = QString::fromStdString(moduleHandler.getFilePath().stem().string());
    emit MessageCenter::getInstance().trackTitleChanged(title);

    updateWindowTitle();

    emit MessageCenter::getInstance().trackDurationChanged(fileInfo.songInfo.songDuration);
    emit moduleFormatChanged(QString::fromStdString(fileInfo.songInfo.songFormat).toUpper());
    emit channelAmountChanged(moduleHandler.getChannelAmount());
    emit activeChannelAmountChanged(moduleHandler.getActiveChannelAmount());
    emit subSongAmountChanged(moduleHandler.getSubSongAmount());
    emit currentSubSongIndexChanged(moduleHandler.getCurrentSubSongIndex());
    emit patternAmountChanged(moduleHandler.getPatternAmount());
    emit currentPatternIndexChanged(moduleHandler.getCurrentPatternIndex());
    ui->timeScrubber->setEnabled(true);
    emit MessageCenter::getInstance().playRequested();
}

void PlayingCenter::onPlayingStarted() {

}

void PlayingCenter::onPlayingStarted(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onPlayingStarted(const PlayListItem playListItem) {

}

void PlayingCenter::onStopped() {

}

void PlayingCenter::onStopped(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onStopped(const PlayListItem playListItem) {

}

void PlayingCenter::onPaused() {

}

void PlayingCenter::onPaused(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onPaused(const PlayListItem playListItem) {

}

void PlayingCenter::onResumed() {

}

void PlayingCenter::onResumed(const SongFileInfo songFileInfo) {

}

void PlayingCenter::onResumed(const PlayListItem playListItem) {

}

void PlayingCenter::onRepeatModeChanged(const RepeatMode repeatMode) {
    parameters->repeatMode = repeatMode;
}

void PlayingCenter::onAmigaFilterChanged(const AmigaFilter amigaFilter)
{

}

void PlayingCenter::onInterpolationFilterChanged(const InterpolationFilter interpolationFilter)
{

}

void PlayingCenter::connectSignalsAndSlots() {

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

    eraseElementFromVector<std::string>(lessKnownExtensions, "mod");
    eraseElementFromVector<std::string>(lessKnownExtensions, "nst");
    eraseElementFromVector<std::string>(lessKnownExtensions, "s3m");
    eraseElementFromVector<std::string>(lessKnownExtensions, "stm");
    eraseElementFromVector<std::string>(lessKnownExtensions, "xm");
    eraseElementFromVector<std::string>(lessKnownExtensions, "it");

    QString lessKnownExtensionListString;
    for(std::string &lessKnownExtension : lessKnownExtensions) {
        lessKnownExtensionListString += QString::fromStdString("*." + lessKnownExtension) + " ";
    }

    return lessKnownExtensionListString.trimmed();
}
