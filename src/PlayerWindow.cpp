/*
PlayerWindow class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>

#include <cassert>
#include <cstddef>

#include <libopenmpt/libopenmpt.hpp>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include <QMimeData>
#include <AndromedaDSP.hpp>
#include "AboutWindow.hpp"
#include <QCloseEvent>
#include "Util/WindowUtil.hpp"
#include <QOverload>
#include <VolumeControl.hpp>
#include <MessageCenter.hpp>
#include "SettingsCenter.hpp"

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow) {
    ui->setupUi(this);
    initializePlayerWindow();
}

void PlayerWindow::setBodyColor(const RGB &backgroundColor, const RGB &textColor) {
    QString style = QString("#PlayerWindow{background-color:%1;}; #PlayerControlButtons{color:%2;}").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    this->setStyleSheet(style);
    ui->centralwidget->setStyleSheet(QString("background-color:%1;").arg(backgroundColor.hex().c_str()));
    ui->timeScrubber->setStyleSheet(QString(".QSlider::handle:horizontal {background-color:%1;}").arg(backgroundColor.hex().c_str()));
    ui->volumeControl->setStyleSheet(QString(".QSlider::handle:vertical {background-color:%1;}").arg(backgroundColor.hex().c_str()));
}

void PlayerWindow::onScrubberStepsAmountChanged(const unsigned int stepsAmount) {
    if(stepsAmount == 0)
        ui->timeScrubber->setMaximum(1);
    else
        ui->timeScrubber->setMaximum((stepsAmount-1));
}

void PlayerWindow::onScrubberPositionChanged(const unsigned int positionIndex) {
    ui->timeScrubber->setValue(positionIndex);
}

PlayerWindow::~PlayerWindow() {
    emit MessageCenter::getInstance().events.soundEvents.volumeChanged(ui->volumeControl->value());

    delete ui;
}

void PlayerWindow::on_timeScrubber_sliderMoved(int position) {
    scrubberClickedPosition = position;
}

void PlayerWindow::updateTimeScrubber() {
    if(playingCenter.getPlayingState() != PlayingState::Stopped)
        if(scrubberClickedPosition != scrubberPreviousValue)
            emit MessageCenter::getInstance().requests.scrubberRequests.scrubbingRequested(scrubberClickedPosition);
    scrubberPreviousValue = scrubberClickedPosition;
}

void PlayerWindow::on_timeScrubber_sliderPressed() {
    scrubberClickedPosition = ui->timeScrubber->value();
    scrubTimer->start();
    emit MessageCenter::getInstance().requests.scrubberRequests.scrubbingRequested(scrubberClickedPosition);
}
void PlayerWindow::on_timeScrubber_sliderReleased() {
    scrubTimer->stop();
}

void PlayerWindow::onModuleHandlerStopped() {
    emit MessageCenter::getInstance().requests.songRequests.stopRequested();
    qDebug()<<"Stop requested";
}

void PlayerWindow::onPlayListEditorShowingStateChanged(bool isShown) {
    ui->actionPlayListEditor->setChecked(isShown);
    ui->optionButtons->togglePlayListEditorButton(isShown);
}

MppParameters * PlayerWindow::getParameters() {
    return SettingsCenter::getInstance().getParameters();
}

void PlayerWindow::resizeEvent(QResizeEvent *event) {
    qDebug()<<"Resize"<<event->size();
}

void PlayerWindow::showEvent(QShowEvent *event) {
    resize(getParameters()->playerWindowSize);
}


void PlayerWindow::onVolumeChanged(const int value) {
    int volumeSliderValue = ui->volumeControl->value();
    if(volumeSliderValue != value)
        ui->volumeControl->setValue(value);
}

void PlayerWindow::onAlwaysOnTopStateChanged(const bool alwaysOnTop) {
    WindowUtil::setAlwaysOnTop(this, alwaysOnTop);
    ui->actionAlways_On_Top->setChecked(alwaysOnTop);
}

void PlayerWindow::onTitleBarHidingStateChanged(const bool hide) {
    if(hide) {
        ui->titleBar->hide();
    }
    else {
        ui->titleBar->show();
    }
    ui->actionHideTitleBar->setChecked(hide);
}

void PlayerWindow::onSnappingToViewPortStateChanged(const bool snapToViewPort) {
    ui->actionSnap_to_Viewport->setChecked(snapToViewPort);
    moveByMouseClick->setSnapToViewPort(snapToViewPort);

}

void PlayerWindow::onSnappingThresholdChanged(const int snappingThreshold) {
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
}

void PlayerWindow::onKeepingStayingInViewPortStateChanged(const bool keepStayingInViewPort) {
    ui->actionKeep_Staying_in_ViewPort->setChecked(keepStayingInViewPort);
    moveByMouseClick->setKeepStayingInViewPort(keepStayingInViewPort);
}

void PlayerWindow::onSettingsChanged() {
    loadSettings();
    spectrumAnalyzerHandler->loadSettings();
    vuMeterHandler->loadSettings();
}

void PlayerWindow::onLoaded(const SongFileInfo songFileInfo, const bool successfull) {
    if(successfull) {
        ui->titleBar->setTitleByFilePath(songFileInfo.filePath);
        ui->timeScrubber->setEnabled(true);
    }
    else {
        ui->titleBar->setTitleByFilePath("");
        ui->timeScrubber->setEnabled(false);
    }
}

void PlayerWindow::onLoaded(const PlayListItem playListItem, const bool successfull) {
    ui->titleBar->setTitleByFilePath(playListItem.songFileInfo.filePath);
    ui->timeScrubber->setEnabled(true);
}

void PlayerWindow::onEqStateChanged(const bool activated) {

}

void PlayerWindow::onDSPStateChanged(const bool activated) {

}

void PlayerWindow::onAmigaFilterChanged(const AmigaFilter amigaFilter) {

}

void PlayerWindow::onInterpolationFilterChanged(const InterpolationFilter interpolationFilter) {

}

void PlayerWindow::onMouseWheelEvent(QPoint angleDelta, bool inverted) {
    //qDebug()<<"Angle Delta:"<<angleDelta<<"Inverted"<<inverted;
    float yDelta = angleDelta.y();
    if(inverted)
        yDelta = -yDelta;
    yDelta *= 0.4;
    ui->volumeControl->setValue(ui->volumeControl->value()+yDelta);
}

void PlayerWindow::onAboutWindowRequested() {
    AboutWindow aboutWindow(this);
    aboutWindow.exec();
}

void PlayerWindow::onRepeatModeToggleRequested() {
    ModPlugPlayer::RepeatMode currentRepeatMode = getParameters()->repeatMode;
    currentRepeatMode++;
    emit MessageCenter::getInstance().requests.songRequests.repeatModeChangeRequested(currentRepeatMode);
}

void PlayerWindow::onAmigaFilterToggleRequested() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    ModPlugPlayer::AmigaFilter currentAmigaFilter = parameters->amigaFilter;
    currentAmigaFilter++;
    emit MessageCenter::getInstance().requests.moduleRequests.amigaFilterChangeRequested(currentAmigaFilter);
}

void PlayerWindow::onInterpolationFilterToggleRequested() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    ModPlugPlayer::InterpolationFilter currentInterpolationFilter = parameters->interpolationFilter;
    currentInterpolationFilter++;
    emit MessageCenter::getInstance().requests.moduleRequests.interpolationFilterChangeRequested(currentInterpolationFilter);
}

void PlayerWindow::onEqToggleRequested() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    emit MessageCenter::getInstance().requests.soundRequests.eqStateChangeRequested(!parameters->eqEnabled);
}

void PlayerWindow::onDSPToggleRequested() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    emit MessageCenter::getInstance().requests.soundRequests.dspStateChangeRequested(!parameters->dspEnabled);
}

void PlayerWindow::onDSPOpToggleRequested() {

}

void PlayerWindow::onPlayListEditorIsHidden() {
    ui->actionPlayListEditor->setChecked(false);
    ui->optionButtons->togglePlayListEditorButton(false);
}

void PlayerWindow::onMinimizeRequested() {
    showMinimized();
}

void PlayerWindow::onMiniPlayerRequested() {

}

void PlayerWindow::onWindowClosingRequested() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    if(parameters->hideByCloseButton) {
        qDebug()<<"Close requested but hide";
        hide();
    }
    else {
        QApplication::exit();
    }
}

void PlayerWindow::onChangeSnapThresholdRequested(int snappingThreshold) {
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
    getParameters()->snappingThreshold = snappingThreshold;
}

void PlayerWindow::onStopped() {
    //spectrumAnalyzerTimer->stop();
}

void PlayerWindow::onPlayingStarted() {
    //spectrumAnalyzerTimer->start();
}

void PlayerWindow::onPlayingStarted(const SongFileInfo songFileInfo) {

}

void PlayerWindow::onPlayingStarted(const PlayListItem playListItem) {

}

void PlayerWindow::onPaused() {

}

void PlayerWindow::onPaused(const SongFileInfo songFileInfo) {

}

void PlayerWindow::onPaused(const PlayListItem playListItem) {

}

void PlayerWindow::onResumed() {

}

void PlayerWindow::onResumed(const SongFileInfo songFileInfo) {

}

void PlayerWindow::onResumed(const PlayListItem playListItem) {

}

void PlayerWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void PlayerWindow::dropEvent(QDropEvent *event) {
    emit MessageCenter::getInstance().requests.songRequests.openRequested(event->mimeData()->urls()[0].toLocalFile().toStdWString());
    event->setDropAction(Qt::LinkAction);
    event->accept();
}

bool PlayerWindow::eventFilter(QObject *watched, QEvent *event) {
    if(watched == ui->timeScrubber) {
        event->accept();
        return false;
    }

    return false;
}

void PlayerWindow::closeEvent (QCloseEvent *event) {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    parameters->playerWindowSize = size();
    if(parameters->hideByCloseButton) {
        hide();
        event->ignore();
    }
    else {
        SettingsCenter::getInstance().saveSettings();
        //parameters->playerWindowSize.save(settings);

        event->accept();
    }
}

