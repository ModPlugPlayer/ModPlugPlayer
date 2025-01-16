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

class PlayerWindow : public QMainWindow,
                     public ModPlugPlayer::Interfaces::Player,
                     public ModPlugPlayer::Interfaces::ModulePlayer
{
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     ModuleHandler moduleHandler;
     void loadSettings();
     void setBodyColor(const RGB &backgroundColor, const RGB &textColor);

     //Player Controls
     int getVolume() const override;
     bool isAlwaysOnTop() const override;
     bool isSnapToViewPort() const override;
     bool isKeptStayingInViewPort() const override;
     bool isTitleBarHidden() const override;

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
     void amigaFilterChangeRequested(const AmigaFilter amigaFilter) override;
     void interpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;

     //Response Signals

     //Module Player Signals
     void amigaFilterChanged(const AmigaFilter amigaFilter) override;
     void interpolationFilterChanged(const InterpolationFilter interpolationFilter) override;
     void moduleFormatChanged(const QString moduleFormat) override;
     void channelAmountChanged(const size_t channelAmount) override;
     void activeChannelAmountChanged(const size_t activeChannelAmount) override;
     void subSongAmountChanged(const size_t subSongAmount) override;
     void currentSubSongIndexChanged(const size_t currentSubSongIndex) override;
     void patternAmountChanged(const size_t patternAmount) override;
     void currentPatternIndexChanged(const size_t currentPatternIndex) override;


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
    void onOpenRequested() override;
    void onOpenRequested(const std::filesystem::path filePath) override;
    void onStopRequested() override;
    void onStopRequested(const SongFileInfo songFileInfo) override;
    void onStopRequested(const PlayListItem playListItem) override;
    void onPlayRequested() override;
    void onPlayRequested(const SongFileInfo songFileInfo) override;
    void onPlayRequested(const PlayListItem playListItem) override;
    void onPauseRequested() override;
    void onPauseRequested(const SongFileInfo songFileInfo) override;
    void onPauseRequested(const PlayListItem playListItem) override;
    void onResumeRequested() override;
    void onResumeRequested(const SongFileInfo songFileInfo) override;
    void onResumeRequested(const PlayListItem playListItem) override;
    void onVolumeChangeRequested(const int volume) override;
    void onTimeScrubbingRequested(const int position) override;
    void onTimeScrubbed(const int position) override;
    void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) override;
    void onTitleBarHidingStateChangeRequested(const bool hide) override;
    void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) override;
    void onSnappingThresholdChangeRequested(const int snappingThreshold) override;
    void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) override;
    void onPreviousRequested() override;
    void onPreviousRequested(const PlayListItem playListItem) override;
    void onNextRequested() override;
    void onNextRequested(const PlayListItem playListItem) override;
    void onRewindRequested() override;
    void onFastForwardRequested() override;
    void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) override;
    void onEqStateChangeRequested(const bool activated) override;
    void onDSPStateChangeRequested(const bool activated) override;
    void onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) override;
    void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;
    void onSetupRequested();

    //Response Signal Handlers
    void onLoaded(const SongFileInfo songFileInfo, const bool successfull) override;
    void onLoaded(const PlayListItem playListItem, bool successfull) override;
    void onPlayingStarted() override;
    void onPlayingStarted(const SongFileInfo songFileInfo) override;
    void onPlayingStarted(const PlayListItem playListItem) override;
    void onStopped() override;
    void onStopped(const SongFileInfo songFileInfo) override;
    void onStopped(const PlayListItem playListItem) override;
    void onPaused() override;
    void onPaused(const SongFileInfo songFileInfo) override;
    void onPaused(const PlayListItem playListItem) override;
    void onResumed() override;
    void onResumed(const SongFileInfo songFileInfo) override;
    void onResumed(const PlayListItem playListItem) override;
    void onRepeatModeChanged(const RepeatMode repeatMode) override;
    void onAmigaFilterChanged(const AmigaFilter amigaFilter) override;
    void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter) override;

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
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void afterLoaded(const SongFileInfo fileInfo);
    MppParameters *parameters = nullptr;
    EventFilters::MoveByMouseClickEventFilter *moveByMouseClick;
    EventFilters::MouseWheelEventFilter *mouseWheel;
    QString getSupportedExtensionsAsString();
    QString getLessKnownSupportedExtensionsAsString();
    template <typename T>
    void eraseElementFromVector(std::vector<T> &myVector, const T &value);
    PlayingMode playingMode = PlayingMode::SingleTrack;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    SongFileInfo currentSongFileInfo; //loaded module file info
    PlayListItem currentPlayListItem; //loaded playlist item info
};

template <typename T>
void PlayerWindow::eraseElementFromVector(std::vector<T> &elements, const T &value){
    elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
}
