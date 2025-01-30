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
#include "ModuleHandler.hpp"
#include <QSettings>
#include <BandFilter.hpp>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QFileDialog>
#include "MppParameters.hpp"
#include <EventFilters.hpp>
#include <SpectrumAnalyzerAnimator>
#include <Interfaces/Player.hpp>
#include <Interfaces/ModulePlayer.hpp>
#include "PlayListEditorWindow.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

using namespace ModPlugPlayer;

class PlayerWindow : public QMainWindow {
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     ModuleHandler moduleHandler;
     void loadSettings();
     void setBodyColor(const RGB &backgroundColor, const RGB &textColor);

     //Player Controls
     int getVolume();
     bool isAlwaysOnTop();
     bool isSnapToViewPort();
     bool isKeptStayingInViewPort();
     bool isTitleBarHidden();

     void setSpectrumAnalyzerType(BarType barType);
     void setVuMeterType(BarType barType);

     void setSpectrumAnalyzerMaximumValue(int maximumValue);
     void setSpectrumAnalyzerLedAmount(int ledAmount);
     void setSpectrumAnalyzerLedHeightRatio(double ledRatio);
     void setSpectrumAnalyzerBarWidthRatio(double barRatio);
     void setSpectrumAnalyzerWindowFunction(WindowFunction windowFunction);
     void setSpectrumAnalyzerDimmingRatio(double dimmingRatio);
     void setSpectrumAnalyzerDimmedTransparencyRatio(double dimmedTransparencyRatio);
     void setSpectrumAnalyzerBarAmount(int barAmount);
     void setSpectrumAnalyzerGradient(const QGradientStops &gradient);
     void setSpectrumAnalyzerScaleToLogarithmic(bool isLogarithmicScale);

     void setVuMeterMaximumValue(int maximumValue);
     void setVuMeterMinimumValue(int minimumValue);
     void setVuMeterLedAmount(int ledAmount);
     void setVuMeterLedHeightRatio(double ledRatio);
     void setVuMeterDimmingRatio(double dimmingRatio);
     void setVuMeterDimmedTransparencyRatio(double dimmedTransparencyRatio);
     void setVuMeterGradient(const QGradientStops &gradient);


     //     static PLAYERSTATE playerState;
     //     static SONGSTATE songState;
signals:
     //Request Signals
     void amigaFilterChangeRequested(const AmigaFilter amigaFilter);
     void interpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter);

     //Response Signals

     //Module Player Signals
     void amigaFilterChanged(const AmigaFilter amigaFilter);
     void interpolationFilterChanged(const InterpolationFilter interpolationFilter);
     void moduleFormatChanged(const QString moduleFormat);
     void channelAmountChanged(const size_t channelAmount);
     void activeChannelAmountChanged(const size_t activeChannelAmount);
     void subSongAmountChanged(const size_t subSongAmount);
     void currentSubSongIndexChanged(const size_t currentSubSongIndex);
     void patternAmountChanged(const size_t patternAmount);
     void currentPatternIndexChanged(const size_t currentPatternIndex);


     //Song signals

public slots:
    void updateTime();
    void updateTimeScrubber();
    void setTimeScrubberTicks(int amount);
    void onAboutWindowRequested();
    void onPlayListEditorWindowRequested(bool turnOn);
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
    void selectNewSoundOutput(PaDeviceIndex deviceIndex);

    // Request Signal Handlers
    void onOpenRequested();
    void onOpenRequested(const std::filesystem::path filePath);
    void onStopRequested();
    void onStopRequested(const SongFileInfo songFileInfo);
    void onStopRequested(const PlayListItem playListItem);
    void onPlayRequested();
    void onPlayRequested(const SongFileInfo songFileInfo);
    void onPlayRequested(const PlayListItem playListItem);
    void onPauseRequested();
    void onPauseRequested(const SongFileInfo songFileInfo);
    void onPauseRequested(const PlayListItem playListItem);
    void onResumeRequested();
    void onResumeRequested(const SongFileInfo songFileInfo);
    void onResumeRequested(const PlayListItem playListItem);
    void onVolumeChangeRequested(const int volume);
    void onTimeScrubbingRequested(const int position);
    void onTimeScrubbed(const int position);
    void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop);
    void onTitleBarHidingStateChangeRequested(const bool hide);
    void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort);
    void onSnappingThresholdChangeRequested(const int snappingThreshold);
    void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort);
    void onPreviousRequested();
    void onPreviousRequested(const PlayListItem playListItem);
    void onNextRequested();
    void onNextRequested(const PlayListItem playListItem);
    void onRewindRequested();
    void onFastForwardRequested();
    void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode);
    void onEqStateChangeRequested(const bool activated);
    void onDSPStateChangeRequested(const bool activated);
    void onAmigaFilterChangeRequested(const AmigaFilter amigaFilter);
    void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter);
    void onSetupRequested();

    //Response Signal Handlers
    void onLoaded(const SongFileInfo songFileInfo, const bool successfull);
    void onLoaded(const PlayListItem playListItem, bool successfull);
    void onPlayingStarted();
    void onPlayingStarted(const SongFileInfo songFileInfo);
    void onPlayingStarted(const PlayListItem playListItem);
    void onStopped();
    void onStopped(const SongFileInfo songFileInfo);
    void onStopped(const PlayListItem playListItem);
    void onPaused();
    void onPaused(const SongFileInfo songFileInfo);
    void onPaused(const PlayListItem playListItem);
    void onResumed();
    void onResumed(const SongFileInfo songFileInfo);
    void onResumed(const PlayListItem playListItem);
    void onRepeatModeChanged(const RepeatMode repeatMode);
    void onAmigaFilterChanged(const AmigaFilter amigaFilter);
    void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter);

private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();
    void onModuleHandlerStopped();

private:
    Ui::PlayerWindow *ui;
    QSettings *settings;
    SpectrumAnalyzerAnimator<double> *spectrumAnalyzerAnimator;
    SpectrumAnalyzerAnimator<double> *vuMeterAnimator;
    static portaudio::System portAudioSystem;
    bool spectrumAlayzerScaleIsLogarithmic = true;
    size_t spectrumAnalyzerBarAmount = 0;
    QTimer *timer = nullptr;
    QTimer *scrubTimer = nullptr;
    QTimer *spectrumAnalyzerTimer = nullptr;
    double timerTimeoutValue = 50;
    double scrubTimerTimeoutValue = 50;
    double spectrumAnalyzerTimerTimeoutValue = 0.1;
    bool scrubberClicked = false;
    int scrubberPreviousValue = 0;
    int scrubberClickedPosition = 0;
    void updateSpectrumAnalyzer();
    double *spectrumData = nullptr;
    QPoint dragPosition;
    QFileDialog *fileDialog = nullptr;
    PlayListEditorWindow *playListEditorWindow = nullptr;
    void connectSignalsAndSlots();
    void initAndInstallEventFilters();
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void initVuMeter();
    void initMenus();
    void updateInstantModuleInfo();
    void updateWindowTitle();
    void onMouseWheelEvent(QPoint angleDelta, bool inverted);
    void resizeEvent(QResizeEvent* event);
    void showEvent(QShowEvent* event);
    void afterLoaded(const SongFileInfo fileInfo);
    MppParameters *parameters = nullptr;
    EventFilters::MoveByMouseClickEventFilter *moveByMouseClick;
    EventFilters::MouseWheelEventFilter *mouseWheel;
    QString getSupportedExtensionsAsString();
    QString getLessKnownSupportedExtensionsAsString();
    template <typename T>
    void eraseElementFromVector(std::vector<T> &myVector, const T &value);
    PlayingMode playingMode = PlayingMode::SingleTrack;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    SongFileInfo currentSongFileInfo; //loaded module file info
    PlayListItem currentPlayListItem; //loaded playlist item info
};

template <typename T>
void PlayerWindow::eraseElementFromVector(std::vector<T> &elements, const T &value){
    elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
}
