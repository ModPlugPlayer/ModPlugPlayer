/*
PlayerWindow class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QMainWindow>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <QSettings>
#include <BandFilter.hpp>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <EventFilters.hpp>
#include <Interfaces/Player.hpp>
#include <Interfaces/ModulePlayer.hpp>
#include "PlayListEditorWindow.hpp"
#include "MppParameters.hpp"
#include "PlayingCenter.hpp"
#include "SpectrumAnalyzerHandler.hpp"
#include "VUMeterHandler.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

using namespace ModPlugPlayer;

class PlayerWindow : public QMainWindow {
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     void setBodyColor(const RGB &backgroundColor, const RGB &textColor);

 public slots:
    void onScrubberStepsAmountChanged(const unsigned int stepsAmount);
    void onScrubberPositionChanged(const unsigned int positionIndex);
    void updateTimeScrubber();
    void onAboutWindowRequested();
    void onRepeatModeToggleRequested();
    void onAmigaFilterToggleRequested();
    void onInterpolationFilterToggleRequested();
    void onEqToggleRequested();
    void onDSPToggleRequested();
    void onDSPOpToggleRequested();
    void onPlayListEditorIsHidden();
    void onMinimizeRequested();
    void onMiniPlayerRequested();
    void onWindowClosingRequested();
    void onChangeSnapThresholdRequested(int snappingThreshold);
    void onStopped();
    void onPlayingStarted();
    void onPlayingStarted(const SongFileInfo songFileInfo);
    void onPlayingStarted(const PlayListItem playListItem);
    void onPaused();
    void onPaused(const SongFileInfo songFileInfo);
    void onPaused(const PlayListItem playListItem);
    void onResumed();
    void onResumed(const SongFileInfo songFileInfo);
    void onResumed(const PlayListItem playListItem);

private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();
    void onModuleHandlerStopped();
    void onPlayListEditorShowingStateChanged(bool isShown);
    void onVolumeChanged(const int value);

    void onAlwaysOnTopStateChanged(const bool alwaysOnTop);
    void onTitleBarHidingStateChanged(const bool hide);
    void onSnappingToViewPortStateChanged(const bool snapToViewPort);
    void onSnappingThresholdChanged(const int snappingThreshold);
    void onKeepingStayingInViewPortStateChanged(const bool keepStayingInViewPort);
    void onSettingsChanged();
    void onLoaded(const ModPlugPlayer::SongFileInfo songFileInfo, const bool successfull);
    void onLoaded(const ModPlugPlayer::PlayListItem playListItem, const bool successfull);
    void onEqStateChanged(const bool activated);
    void onDSPStateChanged(const bool activated);
    void onAmigaFilterChanged(const AmigaFilter amigaFilter);
    void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter);


private:
    Ui::PlayerWindow *ui;
    PlayingCenter &playingCenter = PlayingCenter::getInstance();
    MppParameters * getParameters();
    static portaudio::System portAudioSystem;
    QTimer *scrubTimer = nullptr;
    double scrubTimerTimeoutValue = 50;
    bool scrubberClicked = false;
    int scrubberPreviousValue = 0;
    int scrubberClickedPosition = 0;
    SpectrumAnalyzerHandler *spectrumAnalyzerHandler;
    VUMeterHandler *vuMeterHandler;
    QPoint dragPosition;
    PlayListEditorWindow *playListEditorWindow = nullptr;
    void initializePlayerWindow();
    void loadSettings();
    void connectSignalsAndSlots();
    void connectMenuItems();
    void initAndInstallEventFilters();
    void initAndConnectTimers();
    void initMenus();
    void onMouseWheelEvent(QPoint angleDelta, bool inverted);
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    EventFilters::MoveByMouseClickEventFilter *moveByMouseClick;
    EventFilters::MouseWheelEventFilter *mouseWheel;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
};
