/*
PlayerWindow's initialization method definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>
#include <QOverload>
#include <MessageCenter.hpp>
#include "SettingsCenter.hpp"

using namespace ModPlugPlayer;

void PlayerWindow::initializePlayerWindow() {
#ifdef Q_OS_MACOS
    setUnifiedTitleAndToolBarOnMac(true);

    //Makes the components seem under the transparent titlebar correctly
    setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
    ui->centralwidget->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);

    ui->frame->setStyleSheet("");
#else
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    ui->titleBar->setMinimumHeight(14);
    ui->titleBar->setMaximumHeight(14);
#endif

    QMainWindow::setWindowIcon(QIcon(":/MPP.iconset/icon_512x512.png"));

    setAcceptDrops(true);

    initMenus();

    this->spectrumAnalyzerAnimator = new SpectrumAnalyzerAnimator<double>(20, 0, SettingsCenter::getInstance().getParameters()->spectrumAnalyzerMaximumValue);
    this->vuMeterAnimator = new SpectrumAnalyzerAnimator<double>(1, -40, -8);

    MotionProperties<double> rs, fs, rv, fv;
    rs.acceleration = -10000;
    rs.motionType = MotionType::ConstantAcceleration;

    fs.acceleration = -10000;
    fs.motionType = MotionType::ConstantAcceleration;

    rv.acceleration = -2500;
    fv.acceleration = -2500;
    rv.motionType = MotionType::ConstantAcceleration;
    fv.motionType = MotionType::ConstantAcceleration;

    spectrumAnalyzerAnimator->setFallingMotionProperties(fs);
    spectrumAnalyzerAnimator->setRaisingMotionProperties(rs);
    spectrumAnalyzerAnimator->start();

    vuMeterAnimator->setFallingMotionProperties(fv);
    vuMeterAnimator->setRaisingMotionProperties(rv);
    vuMeterAnimator->start();

    initSpectrumAnalyzer();
    initVuMeter();
    updateSpectrumAnalyzer();


#ifndef Q_OS_MACOS
        //ui->titleBarPlaceHolder->hide();
#endif

    initAndInstallEventFilters();
    connectSignalsAndSlots();
    connectMenuItems();

    initAndConnectTimers();



    ui->centralwidget->setMouseTracking(true);

    int volume = SettingsCenter::getInstance().getParameters()->volume;
    ui->volumeControl->setValue(volume);

    loadSettings();

    ui->timeScrubber->setEnabled(false);
    setFixedSize(413,194);
}

void PlayerWindow::connectSignalsAndSlots()
{
    //ModuleHandler Thread Connections
    //connect(&MessageCenter::getInstance(), qOverload<PlayListItem>(&MessageCenter::openRequested), &this->moduleHandler, qOverload<PlayListItem>(&PlayerWindow::onOpenRequested));

    //connect(this->playListEditorWindow->getPlayListWidget(), qOverload<PlayListItem>(&PlayListWidget::playRequested), &this->moduleHandler, qOverload<PlayListItem>(&ModuleHandler::load));
    //connect(this, qOverload<PlayListItem>(&PlayerWindow::openRequested), &this->moduleHandler, qOverload<PlayListItem>(&ModuleHandler::load));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<SongFileInfo, bool>(&MessageCenterEvents::SongEvents::loaded), this, qOverload<SongFileInfo, bool>(&PlayerWindow::onLoaded));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<PlayListItem, bool>(&MessageCenterEvents::SongEvents::loaded), this, qOverload<PlayListItem, bool>(&PlayerWindow::onLoaded));

    //Option Buttons
    connect(this->ui->optionButtons, &OptionButtons::about, &MessageCenter::getInstance().requests.windowStandardRequests.aboutWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested);
    connect(this->ui->optionButtons, &OptionButtons::playlist, &MessageCenter::getInstance().requests.windowStandardRequests.playlistWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested);
    connect(this->ui->optionButtons, &OptionButtons::repeat, this, &PlayerWindow::onRepeatModeToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::amiga, this, &PlayerWindow::onAmigaFilterToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::filter, this, &PlayerWindow::onInterpolationFilterToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::eq, this, &PlayerWindow::onEqToggleRequested);
    connect(this->ui->optionButtons, &OptionButtons::dsp, this, &PlayerWindow::onDSPToggleRequested);

    connect(this->playListEditorWindow, &PlayListEditorWindow::hidden, this, &PlayerWindow::onPlayListEditorIsHidden);

    connect(&MessageCenter::getInstance().requests.windowStandardRequests.aboutWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested, this, &PlayerWindow::onAboutWindowRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::minimizeRequested, this, &PlayerWindow::onMinimizeRequested);


    connect(&MessageCenter::getInstance().events.windowEvents, &MessageCenterEvents::WindowEvents::alwaysOnTopStateChanged, this, &PlayerWindow::onAlwaysOnTopStateChanged);
    connect(&MessageCenter::getInstance().events.windowEvents, &MessageCenterEvents::WindowEvents::titleBarHidingStateChanged, this, &PlayerWindow::onTitleBarHidingStateChanged);
    connect(&MessageCenter::getInstance().events.windowEvents, &MessageCenterEvents::WindowEvents::snappingToViewPortStateChanged, this, &PlayerWindow::onSnappingToViewPortStateChanged);
    connect(&MessageCenter::getInstance().events.windowEvents, &MessageCenterEvents::WindowEvents::keepingStayingInViewPortStateChanged, this, &PlayerWindow::onKeepingStayingInViewPortStateChanged);


    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::eqStateChanged, this, &PlayerWindow::onEqStateChanged);
    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::dspStateChanged, this, &PlayerWindow::onDSPStateChanged);
    connect(&MessageCenter::getInstance().events.moduleEvents, &MessageCenterEvents::ModuleEvents::amigaFilterChanged, this, &PlayerWindow::onAmigaFilterChanged);
    connect(&MessageCenter::getInstance().events.moduleEvents, &MessageCenterEvents::ModuleEvents::interpolationFilterChanged, this, &PlayerWindow::onInterpolationFilterChanged);

    connect(this->ui->volumeControl, &QSlider::valueChanged, &MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::volumeChangeRequested);
    connect(this->mouseWheel, &EventFilters::MouseWheelEventFilter::mouseWheelEvent, this, &PlayerWindow::onMouseWheelEvent);

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<>(&PlayerWindow::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<const SongFileInfo>(&PlayerWindow::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::playingStarted), this, qOverload<const PlayListItem>(&PlayerWindow::onPlayingStarted));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::stopped),this, qOverload<>(&PlayerWindow::onStopped));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::paused), this, qOverload<>(&PlayerWindow::onPaused));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::paused), this, qOverload<const SongFileInfo>(&PlayerWindow::onPaused));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::paused), this, qOverload<const PlayListItem>(&PlayerWindow::onPaused));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<>(&PlayerWindow::onResumed));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const SongFileInfo>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<const SongFileInfo>(&PlayerWindow::onResumed));
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<const PlayListItem>(&MessageCenterEvents::SongEvents::resumed), this, qOverload<const PlayListItem>(&PlayerWindow::onResumed));

    connect(&MessageCenter::getInstance().events.scrubberEvents, &MessageCenterEvents::ScrubberEvents::scrubberStepsAmountChanged, this, &PlayerWindow::onScrubberStepsAmountChanged);
    connect(&MessageCenter::getInstance().events.scrubberEvents, &MessageCenterEvents::ScrubberEvents::scrubberPositionChanged, this, &PlayerWindow::onScrubberPositionChanged);

    //PlayerWindow Connections
    /*
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::openRequested),this, qOverload<>(&PlayerWindow::onOpenRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::playRequested), this, qOverload<>(&PlayerWindow::onPlayRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::pauseRequested), this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::stopRequested),this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::previousRequested),this, qOverload<>(&PlayerWindow::onPreviousRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::nextRequested),this, qOverload<>(&PlayerWindow::onNextRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::fastForwardRequested),this, qOverload<>(&PlayerWindow::onFastForwardRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::rewindRequested),this, qOverload<>(&PlayerWindow::onRewindRequested));
    connect(&MessageCenter::getInstance().requests.songRequests, qOverload<ModPlugPlayer::PlayListItem>(&PlayListWidget::playRequested),(PlayerWindow *) this->playerWindow, qOverload<ModPlugPlayer::PlayListItem>(&PlayerWindow::onPlayRequested));
    //ToDo: MessageCenter::getInstance().events.settingsEvents.settingsChanged


    connect(&MessageCenter::getInstance(), &MessageCenter::timeScrubbingRequested, this, &PlayerWindow::onTimeScrubbingRequested);
    connect(&MessageCenter::getInstance(), &MessageCenter::timeScrubbed, this, &PlayerWindow::onTimeScrubbed);

    //Repeat Mode Connections
    connect(&MessageCenter::getInstance().requests.songRequests, &MessageCenterRequests::SongRequests::repeatModeChangeRequested, this, &PlayerWindow::onRepeatModeChangeRequested);
    connect(&MessageCenter::getInstance().events.songEvents, &MessageCenterEvents::SongEvents::repeatModeChanged, this, &PlayerWindow::onRepeatModeChanged);

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
    connect(&moduleHandler, &ModuleHandler::timeChanged, this, &PlayerWindow::onElapsedTimeChanged);
    connect(&moduleHandler, &ModuleHandler::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);
    connect(&moduleHandler, &ModuleHandler::stopped, this, &PlayerWindow::onModuleHandlerStopped);
    connect(&moduleHandler, &ModuleHandler::playerStateChanged, ui->playerControlButtons, &PlayerControlButtons::on_playerState_changed);

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
    connect(&MessageCenter::getInstance(), &MessageCenter::songDurationChanged, ui->lcdPanel, &LCDDisplay::onSongDurationChanged);
    connect(&MessageCenter::getInstance(), &MessageCenter::songTitleChanged, ui->lcdPanel, &LCDDisplay::onSongTitleChanged);
    connect(this, &PlayerWindow::moduleFormatChanged, ui->lcdPanel, &LCDDisplay::onModuleFormatChanged);
    connect(this, &PlayerWindow::channelAmountChanged, ui->lcdPanel, &LCDDisplay::onChannelAmountChanged);
    connect(this, &PlayerWindow::activeChannelAmountChanged, ui->lcdPanel, &LCDDisplay::onActiveChannelAmountChanged);
    connect(this, &PlayerWindow::subSongAmountChanged, ui->lcdPanel, &LCDDisplay::onSubSongAmountChanged);
    connect(this, &PlayerWindow::currentSubSongIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentSubSongChanged);
    connect(this, &PlayerWindow::patternAmountChanged, ui->lcdPanel, &LCDDisplay::onPatternAmountChanged);
    connect(this, &PlayerWindow::currentPatternIndexChanged, ui->lcdPanel, &LCDDisplay::onCurrentPatternChanged);
    */
}

void PlayerWindow::connectMenuItems() {
    //Menu Items
    connect(this->ui->actionOpen, &QAction::triggered, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::openRequested));
    connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, &MessageCenter::getInstance().requests.windowStandardRequests.aboutWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested);
    connect(this->ui->actionSetup, &QAction::triggered, &MessageCenter::getInstance().requests.windowStandardRequests.settingsWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested);
    connect(this->ui->actionPlay, &QAction::triggered, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::playRequested));
    connect(this->ui->actionPause, &QAction::triggered, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::pauseRequested));
    connect(this->ui->actionStop, &QAction::triggered, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::stopRequested));
    connect(this->ui->actionMinimize, &QAction::triggered, &MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::minimizeRequested);
    connect(this->ui->actionPlayListEditor, &QAction::toggled, [=](bool activated) {
        if(activated)
            emit MessageCenter::getInstance().requests.windowStandardRequests.playlistWindowRequests.windowOpenRequested();
        else
            emit MessageCenter::getInstance().requests.windowStandardRequests.playlistWindowRequests.windowCloseRequested();
    });

    connect(this->ui->actionAlways_On_Top, &QAction::toggled, &MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::alwaysOnTopStateChangeRequested);
    connect(this->ui->actionHideTitleBar, &QAction::toggled, &MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::titleBarHidingStateChangeRequested);
    connect(this->ui->actionSnap_to_Viewport, &QAction::toggled, &MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::snappingToViewPortStateChangeRequested);
    connect(this->ui->actionKeep_Staying_in_ViewPort, &QAction::toggled, &MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::keepingStayingInViewPortStateChangeRequested);
}

void PlayerWindow::initAndInstallEventFilters() {
    moveByMouseClick = new EventFilters::MoveByMouseClickEventFilter(this);
    mouseWheel = new EventFilters::MouseWheelEventFilter();

    ui->centralwidget->installEventFilter(this);
    ui->centralwidget->installEventFilter(mouseWheel);
    ui->volumeControl->installEventFilter(mouseWheel);
    ui->timeScrubber->installEventFilter(mouseWheel);
    ui->lcdPanel->installEventFilter(moveByMouseClick);
    ui->spectrumAnalyzerFrame->installEventFilter(moveByMouseClick);
    ui->vuMeterFrame->installEventFilter(moveByMouseClick);
    //ui->centralwidget->installEventFilter(keepFixedSize);
    ui->titleBar->installEventFilter(moveByMouseClick);
}

void PlayerWindow::initAndConnectTimers() {
    timer = new QTimer(this);
    scrubTimer = new QTimer(this);
    spectrumAnalyzerTimer = new QTimer(this);
    spectrumAnalyzerTimer->setInterval(spectrumAnalyzerTimerTimeoutValue);
    spectrumAnalyzerTimer->start();
    //connect(timer, &QTimer::timeout, this, &PlayerWindow::onElapsedTimeChanged);
    connect(scrubTimer, &QTimer::timeout, this, &PlayerWindow::updateTimeScrubber);
    connect(spectrumAnalyzerTimer, &QTimer::timeout, this, &PlayerWindow::updateSpectrumAnalyzer);
    timer->setInterval(timerTimeoutValue);
    timer->start();
}

void PlayerWindow::initSpectrumAnalyzer() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    SpectrumAnalyzerParameters spectrumAnalyzerParameters;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumData = new double[spectrumAnalyzerParameters.barAmount];

    std::fill(spectrumData, spectrumData + spectrumAnalyzerParameters.barAmount, 0);

    spectrumAnalyzerParameters.barDirection = Qt::Orientation::Vertical;
    spectrumAnalyzerParameters.peakValue = parameters->spectrumAnalyzerMaximumValue;
    spectrumAnalyzerParameters.barWidthRatio = parameters->spectrumAnalyzerBarWidthRatio;
    spectrumAnalyzerParameters.dimmingRatio = parameters->spectrumAnalyzerDimmingRatio*100;
    spectrumAnalyzerParameters.dimmedTransparencyRatio = parameters->spectrumAnalyzerDimmedTransparencyRatio*100;
    spectrumAnalyzerParameters.discreteParameters.ledHeightRatio = parameters->spectrumAnalyzerLedHeightRatio;;
    spectrumAnalyzerParameters.discreteParameters.barLedAmount = getParameters()->spectrumAnalyzerLedAmount;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumAnalyzerParameters.gradientStops = parameters->spectrumAnalyzerGradient;

    ui->spectrumAnalyzer->setParameters(spectrumAnalyzerParameters);
}

void PlayerWindow::initVuMeter() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    SpectrumAnalyzerParameters vuMeterParameters;

    vuMeterParameters.barDirection = Qt::Orientation::Vertical;
    vuMeterParameters.barAmount = 1;

    vuMeterParameters.peakValue = parameters->vuMeterMaximumValue;
    vuMeterParameters.floorValue = parameters->vuMeterMinimumValue;
    vuMeterParameters.barWidthRatio = 1;
    vuMeterParameters.dimmingRatio = parameters->vuMeterDimmingRatio*100;
    vuMeterParameters.dimmedTransparencyRatio = parameters->vuMeterDimmedTransparencyRatio*100;
    vuMeterParameters.discreteParameters.ledHeightRatio = parameters->vuMeterLedHeightRatio;;
    vuMeterParameters.discreteParameters.barLedAmount = parameters->vuMeterLedAmount;
    vuMeterParameters.gradientStops = parameters->vuMeterGradient;
    ui->vuMeter->setParameters(vuMeterParameters);
}

void PlayerWindow::initMenus() {
    ui->menubar->hide();

    ui->actionAbout_ModPlug_Player->setMenuRole(QAction::ApplicationSpecificRole);
    QAction * aboutSeparator = ui->menuFile->addSeparator();
    aboutSeparator->setMenuRole(QAction::ApplicationSpecificRole);

    ui->actionSetup->setMenuRole(QAction::ApplicationSpecificRole);
}
