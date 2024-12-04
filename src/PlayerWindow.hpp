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
#include "ModulePlayer.hpp"
#include <QSettings>
#include <BandFilter.hpp>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QFileDialog>
#include "MppParameters.hpp"
#include "EventFilters.hpp"
#include <SpectrumAnalyzerAnimator>
#include <Player.hpp>
#include "PlayListEditorWindow.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

using namespace ModPlugPlayer;

class PlayerWindow : public QMainWindow, public ModPlugPlayer::Player
{
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     ModulePlayer modulePlayer;
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
     void openRequested(const std::filesystem::path filePath) override;
     void openRequested(const PlayListItem playListItem) override;
     void stopRequested() override;
     void stopRequested(const PlayListItem playListItem) override;
     void playRequested() override;
     void playRequested(const PlayListItem playListItem) override;
     void pauseRequested() override;
     void pauseRequested(const PlayListItem playListItem) override;
     void resumeRequested() override;
     void resumeRequested(const PlayListItem playListItem) override;
     void previousRequested() override;
     void nextRequested() override;
     void volumeChangeRequested(const int volume) override;
     void timeScrubbingRequested(const int position) override;
     void repeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) override;
     void eqStateChangeRequested(const bool activated) override;
     void agcStateChangeRequested(const bool activated) override;
     void xBassStateChangeRequested(const bool activated) override;
     void surroundStateChangeRequested(const bool activated) override;
     void reverbStateChangeRequested(const bool activated) override;
     void interpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;
     void alwaysOnTopStateChangeRequested(const bool alwaysOnTop) override;
     void titleBarHidingStateChangeRequested(const bool hide) override;
     void snappingToViewPortStateChangeRequested(const bool toBeSnappedToViewPort) override;
     void keepingStayingInViewPortStateChangeRequested(const bool toBeKeptStayingInViewPort) override;

     //Response Signals
     void loaded(const std::filesystem::path filePath, bool successfull) override;
     void loaded(const PlayListItem playListItem, bool successfull) override;
     void stopped() override;
     void stopped(const PlayListItem playListItem) override;
     void playingStarted() override;
     void playingStarted(const PlayListItem playListItem) override;
     void paused() override;
     void paused(const PlayListItem playListItem) override;
     void resumed() override;
     void resumed(const PlayListItem playListItem) override;
     void previous() override;
     void next() override;
     void volumeChanged(const int volume) override;
     void timeScrubbed(const int position) override;
     void repeatModeChanged(const ModPlugPlayer::RepeatMode repeat) override;
     void eqStateChanged(const bool activated) override;
     void agcStateChanged(const bool activated) override;
     void xBassStateChanged(const bool activated) override;
     void surroundStateChanged(const bool activated) override;
     void reverbStateChanged(const bool activated) override;
     void interpolationFilterChanged(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;
     void alwaysOnTopStateChanged(const bool alwaysOnTop) override;
     void titleBarHidingStateChanged(const bool hide) override;
     void snappingToViewPortStateChanged(const bool snapToViewPort) override;
     void keepingStayingInViewPortStateChanged(const bool toBeKeptStayingInViewPort) override;

public slots:
    void updateTime();
    void updateTimeScrubber();
    void setTimeScrubberTicks(int amount);
    void onPreferencesWindowRequested();
    void onFileOpeningRequested();
    void onAboutWindowRequested();
    void onPlayListEditorWindowRequested(bool turnOn);
    void onRepeatModeToggleRequested();
    void onPlayListEditorIsHidden();
    void onMinimizeRequested();
    void onMiniPlayerRequested();
    void onWindowClosingRequested();
    void onChangeSnapThresholdRequested(int snappingThreshold);
    void selectNewSoundOutput(PaDeviceIndex deviceIndex);

    // Request Signal Handlers
    void onOpenRequested(const std::filesystem::path filePath) override;
    void onOpenRequested(const PlayListItem playListItem) override;
    void onLoaded(const std::filesystem::path filePath, const bool successfull) override;
    void onLoaded(const ModuleFileInfo fileInfo, const bool successfull); //Temporary, will be removed
    void onLoaded(const PlayListItem playListItem, bool successfull) override;
    void onStopRequested() override;
    void onStopRequested(const PlayListItem playListItem) override;
    void onPlayRequested() override;
    void onPlayRequested(const PlayListItem playListItem) override;
    void onPauseRequested() override;
    void onPauseRequested(const PlayListItem playListItem) override;
    void onResumeRequested() override;
    void onResumeRequested(const PlayListItem playListItem) override;
    void onVolumeChangeRequested(const int volume) override;
    void onTimeScrubbingRequested(const int position) override;
    void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) override;
    void onTitleBarHidingStateChangeRequested(const bool hide) override;
    void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) override;
    void onSnappingThresholdChangeRequested(const int snappingThreshold) override;
    void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) override;
    void onPreviousRequested() override;
    void onNextRequested() override;
    void onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) override;
    void onEqStateChangeRequested(const bool activated) override;
    void onAGCStateChangeRequested(const bool activated) override;
    void onXBassStateChangeRequested(const bool activated) override;
    void onSurroundStateChangeRequested(const bool activated) override;
    void onReverbStateChangeRequested(const bool activated) override;
    void onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter) override;

private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();

private:
    Ui::PlayerWindow *ui;
    QSettings *settings;
    SpectrumAnalyzerAnimator<double> *spectrumAnalyzerAnimator;
    SpectrumAnalyzerAnimator<double> *vuMeterAnimator;
    static portaudio::System portAudioSystem;
    QTimer *timer;
    QTimer *scrubTimer;
    QTimer *spectrumAnalyzerTimer;
    double timerTimeoutValue = 50;
    double scrubTimerTimeoutValue = 50;
    double spectrumAnalyzerTimerTimeoutValue = 0.1;
    bool scrubberClicked = false;
    int scrubberPreviousValue = 0;
    int scrubberClickedPosition;
    void updateSpectrumAnalyzer();
    double *spectrumData;
    QPoint dragPosition;
    QFileDialog *fileDialog;
    PlayListEditorWindow *playListEditorWindow;
    void connectSignalsAndSlots();
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void initVuMeter();
    void initMenus();
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
	MppParameters *parameters;
	MoveByMouseClickEventFilter *moveByMouseClick;
    QString getSupportedExtensionsAsString();
    QString getLessKnownSupportedExtensionsAsString();
    template <typename T>
    void eraseElementFromVector(std::vector<T> &myVector, const T &value);
    PlayingMode playingMode = PlayingMode::SingleTrack;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
};

template <typename T>
void PlayerWindow::eraseElementFromVector(std::vector<T> &elements, const T &value){
    elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
}
