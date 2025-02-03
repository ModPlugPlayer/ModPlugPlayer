/*
PlayerWindow's signal and slot connection method definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>
#include "ModuleHandler.hpp"
#include <QOverload>
#include <MessageCenter.hpp>

using namespace ModPlugPlayer;

void PlayerWindow::connectSignalsAndSlots()
{
    //ModuleHandler Thread Connections
    //connect(&MessageCenter::getInstance(), qOverload<PlayListItem>(&MessageCenter::openRequested), &this->moduleHandler, qOverload<PlayListItem>(&PlayerWindow::onOpenRequested));

    //connect(this->playListEditorWindow->getPlayListWidget(), qOverload<PlayListItem>(&PlayListWidget::playRequested), &this->moduleHandler, qOverload<PlayListItem>(&ModuleHandler::load));
    //connect(this, qOverload<PlayListItem>(&PlayerWindow::openRequested), &this->moduleHandler, qOverload<PlayListItem>(&ModuleHandler::load));

    connect(&MessageCenter::getInstance(), qOverload<SongFileInfo, bool>(&MessageCenter::loaded), this, qOverload<SongFileInfo, bool>(&PlayerWindow::onLoaded));
    connect(&MessageCenter::getInstance(), qOverload<PlayListItem, bool>(&MessageCenter::loaded), this, qOverload<PlayListItem, bool>(&PlayerWindow::onLoaded));

    //Option Buttons
    connect(this->ui->optionButtons, &OptionButtons::about, this, &PlayerWindow::onAboutWindowRequested);
    connect(this->ui->optionButtons, &OptionButtons::playlist, this, &PlayerWindow::onPlayListEditorWindowRequested);
    connect(this->ui->optionButtons, &OptionButtons::repeat, this, &PlayerWindow::onRepeatModeToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::amiga, this, &PlayerWindow::onAmigaFilterToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::filter, this, &PlayerWindow::onInterpolationFilterToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::eq, this, &PlayerWindow::onEqToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::dsp, this, &PlayerWindow::onDSPToggleRequested);

    connect(this->playListEditorWindow, &PlayListEditorWindow::hidden, this, &PlayerWindow::onPlayListEditorIsHidden);

    //PlayerWindow Connections
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::openRequested),this, qOverload<>(&PlayerWindow::onOpenRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::playRequested), this, qOverload<>(&PlayerWindow::onPlayRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::pauseRequested), this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::stopRequested),this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::setupRequested), this, qOverload<>(&PlayerWindow::onSetupRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::previousRequested),this, qOverload<>(&PlayerWindow::onPreviousRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::nextRequested),this, qOverload<>(&PlayerWindow::onNextRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::fastForwardRequested),this, qOverload<>(&PlayerWindow::onFastForwardRequested));
    connect(&MessageCenter::getInstance(), qOverload<>(&MessageCenter::rewindRequested),this, qOverload<>(&PlayerWindow::onRewindRequested));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::playRequested),(PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::onPlayRequested));
    //ToDo: MessageCenter::getInstance().events.settingsEvents.settingsChanged


    connect(&MessageCenter::getInstance(), &MessageCenter::timeScrubbingRequested, this, &PlayerWindow::onTimeScrubbingRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::timeScrubbed, this, &PlayerWindow::onTimeScrubbed);

    //Repeat Mode Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::repeatModeChangeRequested, this, &PlayerWindow::onRepeatModeChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::repeatModeChanged, this, &PlayerWindow::onRepeatModeChanged);

    //AmigaFilter Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::amigaFilterChangeRequested, this, &PlayerWindow::onAmigaFilterChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::amigaFilterChanged, ui->lcdPanel, &LCDDisplay::onAmigaFilterChanged);

    //InterpolationFilter Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::interpolationFilterChangeRequested, this, &PlayerWindow::onInterpolationFilterChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::interpolationFilterChanged, ui->lcdPanel, &LCDDisplay::onInterpolationFilterChanged);

    //Eq Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::eqStateChangeRequested, this, &PlayerWindow::onEqStateChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::eqStateChanged, ui->lcdPanel, &LCDDisplay::onEqStateChanged);

    //DSP Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::dspStateChangeRequested, this, &PlayerWindow::onDSPStateChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::dspStateChanged, ui->lcdPanel, &LCDDisplay::onDSPStateChanged);

    //Module Handler
    connect(&moduleHandler, &ModuleHandler::timeChanged, this, &PlayerWindow::updateTime);
    connect(&moduleHandler, &ModuleHandler::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);
    connect(&moduleHandler, &ModuleHandler::stopped, this, &PlayerWindow::onModuleHandlerStopped);
    connect(&moduleHandler, &ModuleHandler::playerStateChanged, ui->playerControlButtons, &PlayerControlButtons::on_playerState_changed);

    //Menu Items
    connect(this->ui->actionOpen, &QAction::triggered, &MessageCenter::getInstance(), qOverload<>(&MessageCenter::openRequested));
    connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, this, &PlayerWindow::onAboutWindowRequested);
    connect(this->ui->actionPreferences, &QAction::triggered, &MessageCenter::getInstance(), &MessageCenter::setupRequested);
    connect(this->ui->actionPlay, &QAction::triggered, &MessageCenter::getInstance(), qOverload<>(&MessageCenter::playRequested));
    connect(this->ui->actionPause, &QAction::triggered, &MessageCenter::getInstance(), qOverload<>(&MessageCenter::pauseRequested));
    connect(this->ui->actionStop, &QAction::triggered, &MessageCenter::getInstance(), qOverload<>(&MessageCenter::stopRequested));
    connect(this->ui->actionPlay, &QAction::triggered, this, qOverload<>(&PlayerWindow::onPlayRequested));
    connect(this->ui->actionPause, &QAction::triggered, this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(this->ui->actionStop, &QAction::triggered, this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(this->ui->actionMinimize, &QAction::triggered, this, &PlayerWindow::onMinimizeRequested);
    connect(this->ui->actionPlayListEditor, &QAction::toggled, this, &PlayerWindow::onPlayListEditorWindowRequested);
    connect(this->ui->actionAlways_On_Top, &QAction::toggled, this, &PlayerWindow::onAlwaysOnTopStateChangeRequested);
    connect(this->ui->actionHideTitleBar, &QAction::toggled, this, &PlayerWindow::onTitleBarHidingStateChangeRequested);
    connect(this->ui->actionSnap_to_Viewport, &QAction::toggled, this, &PlayerWindow::onSnappingToViewPortStateChangeRequested);
    connect(this->ui->actionKeep_Staying_in_ViewPort, &QAction::toggled, this, &PlayerWindow::onKeepingStayingInViewPortStateChangeRequested);

    connect(this->ui->volumeControl, &QSlider::valueChanged, this, &PlayerWindow::onVolumeChangeRequested);
    connect(this->mouseWheel, &EventFilters::MouseWheelEventFilter::mouseWheelEvent, this, &PlayerWindow::onMouseWheelEvent);

    connect(this->ui->titleBar, &TitleBar::minimizeButtonClicked, this, &PlayerWindow::onMinimizeRequested);
    connect(this->ui->titleBar, &TitleBar::miniPlayerButtonClicked, this, &PlayerWindow::onMiniPlayerRequested);
    connect(this->ui->titleBar, &TitleBar::closeButtonClicked, this, &PlayerWindow::onWindowClosingRequested);


    //LCD Display Properties Area Connections
    connect(&MessageCenter::getInstance(), &MessageCenter::repeatModeChanged, ui->lcdPanel, &LCDDisplay::onRepeatModeChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::eqStateChanged, ui->lcdPanel, &LCDDisplay::onEqStateChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::dspStateChanged, ui->lcdPanel, &LCDDisplay::onDSPStateChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::amigaFilterChanged, ui->lcdPanel, &LCDDisplay::onAmigaFilterChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::interpolationFilterChanged, ui->lcdPanel, &LCDDisplay::onInterpolationFilterChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::elapsedTimeChanged, ui->lcdPanel, &LCDDisplay::onElapsedTimeChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::trackDurationChanged, ui->lcdPanel, &LCDDisplay::onTrackDurationChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::trackTitleChanged, ui->lcdPanel, &LCDDisplay::onTrackTitleChanged);
    connect(this, &PlayerWindow::moduleFormatChanged, ui->lcdPanel, &LCDDisplay::onModuleFormatChanged);
    connect(this, &PlayerWindow::channelAmountChanged, ui->lcdPanel, &LCDDisplay::onChannelAmountChanged);
    connect(this, &PlayerWindow::activeChannelAmountChanged, ui->lcdPanel, &LCDDisplay::onActiveChannelAmountChanged);
    connect(this, &PlayerWindow::subSongAmountChanged, ui->lcdPanel, &LCDDisplay::onSubSongAmountChanged);
    connect(this, &PlayerWindow::currentSubSongIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentSubSongChanged);
    connect(this, &PlayerWindow::patternAmountChanged, ui->lcdPanel, &LCDDisplay::onPatternAmountChanged);
    connect(this, &PlayerWindow::currentPatternIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentPatternChanged);

    connect(&MessageCenter::getInstance(), &MessageCenter::eqStateChangeRequested, this, &PlayerWindow::onEqStateChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::dspStateChangeRequested, this, &PlayerWindow::onDSPStateChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::amigaFilterChangeRequested, this, &PlayerWindow::onAmigaFilterChangeRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::interpolationFilterChangeRequested, this, &PlayerWindow::onInterpolationFilterChangeRequested);
}
