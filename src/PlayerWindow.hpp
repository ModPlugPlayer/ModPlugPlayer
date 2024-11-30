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
     void open(std::filesystem::path filePath) override;
     void open(const PlayListItem playListItem) override;
     void stop() override;
     void stop( PlayListItem playListItem) override;
     void play() override;
     void play( PlayListItem playListItem) override;
     void pause() override;
     void pause( PlayListItem playListItem) override;
     void resume() override;
     void resume( PlayListItem playListItem) override;
     void previous() override;
     void next() override;
     void changeVolume(int volume) override;
     void scrubTime(int position) override;
     void changeRepeat(ModPlugPlayer::RepeatState repeatstate) override;
     void setAlwaysOnTop(bool alwaysOnTop) override;
     void hideTitleBar(bool hide) override;
     void snapToViewPort(bool toBeSnappedToViewPort) override;
     void keepStayingInViewPort(bool toBeKeptStayingInViewPort) override;

public slots:
    void updateTime();
    void updateTimeScrubber();
    void setTimeScrubberTicks(int amount);
    void onPreferencesWindowRequested();
    void onFileLoaded();
    void onFileOpeningRequested();
    void onAboutWindowRequested();
    void onPlayListEditorWindowRequested(bool turnOn);
    void onRepeatModeChangeRequested();
    void onPlayListEditorIsHidden();
    void onMinimizeRequested();
    void onMiniPlayerRequested();
    void onWindowClosingRequested();
    void onKeepStayingViewPortRequested(bool keepStayingInViewPort);
    void onChangeSnapThresholdRequested(int snappingThreshold);
    void selectNewSoundOutput(PaDeviceIndex deviceIndex);

    // Player Controls
    void onOpen(std::filesystem::path filePath) override;
    void onOpen(PlayListItem playListItem) override;
    void onStop() override;
    void onStop(PlayListItem playListItem) override;
    void onPlay() override;
    void onPlay(PlayListItem playListItem) override;
    void onPause() override;
    void onPause(PlayListItem playListItem) override;
    void onResume() override;
    void onResume(PlayListItem playListItem) override;
    void onChangeVolume(int volume) override;
    void onScrubTime(int position) override;
    void onSetAlwaysOnTop(bool alwaysOnTop) override;
    void onHideTitleBar(bool hide) override;
    void onSetSnapToViewPort(bool snapToViewPort) override;
    void onSetSnappingThreshold(int snappingThreshold) override;
    void onSetKeepStayingInViewPort(bool keepStayingInViewPort) override;
    void onPrevious() override;
    void onNext() override;
    void onChangeRepeat(ModPlugPlayer::RepeatState repeat) override;

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
