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

using namespace ModPlugPlayer;

void PlayerWindow::connectSignalsAndSlots()
{
    //ModuleHandler Thread Connections
    connect(this, qOverload<std::filesystem::path>(&PlayerWindow::openRequested), &this->moduleHandler, qOverload<std::filesystem::path>(&ModuleHandler::load));
    connect(this, qOverload<PlayListItem>(&PlayerWindow::openRequested), &this->moduleHandler, qOverload<PlayListItem>(&ModuleHandler::load));
    connect(&this->moduleHandler, &ModuleHandler::moduleFileLoaded, this, qOverload<SongFileInfo, bool>(&PlayerWindow::onLoaded));

    //Player Control Buttons
    //connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, &moduleHandler, &ModuleHandler::stop);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, &moduleHandler, &ModuleHandler::pause);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::play, &moduleHandler, &ModuleHandler::play);
    //    connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, &moduleHandler, &ModuleHandler::resume);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::onPreferencesWindowRequested);

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
    connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::onFileOpeningRequested);
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::stop), this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::pause), this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::play), this, qOverload<>(&PlayerWindow::onPlayRequested));
    connect(this, qOverload<>(&PlayerWindow::stopRequested),this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(this, &PlayerWindow::timeScrubbingRequested, this, &PlayerWindow::onTimeScrubbingRequested);
    connect(this, &PlayerWindow::timeScrubbed, this, &PlayerWindow::onTimeScrubbed);
    //Repeat Mode Connections
    connect(this, &PlayerWindow::repeatModeChangeRequested, this, &PlayerWindow::onRepeatModeChangeRequested);
    connect(this, &PlayerWindow::repeatModeChanged, this, &PlayerWindow::onRepeatModeChanged);

    //AmigaFilter Connections
    connect(this, &PlayerWindow::amigaFilterChangeRequested, this, &PlayerWindow::onAmigaFilterChangeRequested);
    connect(this, &PlayerWindow::amigaFilterChanged, ui->lcdPanel, &LCDDisplay::onAmigaFilterChanged);

    //InterpolationFilter Connections
    connect(this, &PlayerWindow::interpolationFilterChangeRequested, this, &PlayerWindow::onInterpolationFilterChangeRequested);
    connect(this, &PlayerWindow::interpolationFilterChanged, ui->lcdPanel, &LCDDisplay::onInterpolationFilterChanged);

    //Eq Connections
    connect(this, &PlayerWindow::eqStateChangeRequested, this, &PlayerWindow::onEqStateChangeRequested);
    connect(this, &PlayerWindow::eqStateChanged, ui->lcdPanel, &LCDDisplay::onEqStateChanged);

    //DSP Connections
    connect(this, &PlayerWindow::dspStateChangeRequested, this, &PlayerWindow::onDSPStateChangeRequested);
    connect(this, &PlayerWindow::dspStateChanged, ui->lcdPanel, &LCDDisplay::onDSPStateChanged);

    //Module Handler
    connect(&moduleHandler, &ModuleHandler::timeChanged, this, &PlayerWindow::updateTime);
    connect(&moduleHandler, &ModuleHandler::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);
    connect(&moduleHandler, &ModuleHandler::stopped, this, &PlayerWindow::onModuleHandlerStopped);
    connect(&moduleHandler, &ModuleHandler::playerStateChanged, ui->playerControlButtons, &PlayerControlButtons::on_playerState_changed);

    //Menu Items
    connect(this->ui->actionOpen, &QAction::triggered, this, &PlayerWindow::onFileOpeningRequested);
    connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, this, &PlayerWindow::onAboutWindowRequested);
    connect(this->ui->actionPreferences, &QAction::triggered, this, &PlayerWindow::onPreferencesWindowRequested);
    connect(this->ui->actionPlay, &QAction::triggered, &moduleHandler, &ModuleHandler::play);
    connect(this->ui->actionPause, &QAction::triggered, &moduleHandler, &ModuleHandler::pause);
    connect(this->ui->actionStop, &QAction::triggered, &moduleHandler, &ModuleHandler::stop);
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
    connect(this, &PlayerWindow::repeatModeChanged, ui->lcdPanel, &LCDDisplay::onRepeatModeChanged);
    connect(this, &PlayerWindow::eqStateChanged, ui->lcdPanel, &LCDDisplay::onEqStateChanged);
    connect(this, &PlayerWindow::dspStateChanged, ui->lcdPanel, &LCDDisplay::onDSPStateChanged);
    connect(this, &PlayerWindow::amigaFilterChanged, ui->lcdPanel, &LCDDisplay::onAmigaFilterChanged);
    connect(this, &PlayerWindow::interpolationFilterChanged, ui->lcdPanel, &LCDDisplay::onInterpolationFilterChanged);
    connect(this, &PlayerWindow::elapsedTimeChanged, ui->lcdPanel, &LCDDisplay::onElapsedTimeChanged);
    connect(this, &PlayerWindow::trackDurationChanged, ui->lcdPanel, &LCDDisplay::onTrackDurationChanged);
    connect(this, &PlayerWindow::trackTitleChanged, ui->lcdPanel, &LCDDisplay::onTrackTitleChanged);
    connect(this, &PlayerWindow::moduleFormatChanged, ui->lcdPanel, &LCDDisplay::onModuleFormatChanged);
    connect(this, &PlayerWindow::channelAmountChanged, ui->lcdPanel, &LCDDisplay::onChannelAmountChanged);
    connect(this, &PlayerWindow::activeChannelAmountChanged, ui->lcdPanel, &LCDDisplay::onActiveChannelAmountChanged);
    connect(this, &PlayerWindow::subSongAmountChanged, ui->lcdPanel, &LCDDisplay::onSubSongAmountChanged);
    connect(this, &PlayerWindow::currentSubSongIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentSubSongChanged);
    connect(this, &PlayerWindow::patternAmountChanged, ui->lcdPanel, &LCDDisplay::onPatternAmountChanged);
    connect(this, &PlayerWindow::currentPatternIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentPatternChanged);

    connect(ui->lcdPanel, &LCDDisplay::repeatModeChangeRequested, this, &PlayerWindow::repeatModeChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::eqStateChangeRequested, this, &PlayerWindow::onEqStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::dspStateChangeRequested, this, &PlayerWindow::onDSPStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::amigaFilterChangeRequested, this, &PlayerWindow::onAmigaFilterChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::interpolationFilterChangeRequested, this, &PlayerWindow::onInterpolationFilterChangeRequested);
}
