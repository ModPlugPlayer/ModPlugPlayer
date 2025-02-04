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
#include <SpectrumAnalyzerAnimator>
#include <SpectrumAnalyzer.hpp>
#include <Interfaces/Player.hpp>
#include <Interfaces/ModulePlayer.hpp>
#include "PlayListEditorWindow.hpp"
#include "MppParameters.hpp"

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

     void setSpectrumAnalyzerType(BarType barType);
     void setVuMeterType(BarType barType);

     void setSpectrumAnalyzerMaximumValue(int maximumValue);
     void setSpectrumAnalyzerLedAmount(int ledAmount);
     void setSpectrumAnalyzerLedHeightRatio(double ledRatio);
     void setSpectrumAnalyzerBarWidthRatio(double barRatio);
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

     bool isTitleBarHidden() const;
 public slots:
    void updateTime();
    void updateTimeScrubber();
    void setTimeScrubberTicks(int amount);
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
    void onStopRequested();
    void onPlayRequested();
    void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop);

private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();
    void onModuleHandlerStopped();
    void onPlayListEditorShowingStateChanged(bool isShown);
    void onTitleBarHidingStateChangeRequested(bool hide);
    void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort);
    void onVolumeChanged(const int value);

    void onAlwaysOnTopStateChanged(const bool alwaysOnTop);
    void onTitleBarHidingStateChanged(const bool hide);
    void onSnappingToViewPortStateChanged(const bool snapToViewPort);
    void onSnappingThresholdChanged(const int snappingThreshold);
    void onKeepingStayingInViewPortStateChanged(const bool keepStayingInViewPort);
    void onSettingsChanged();
    void onLoaded(const ModPlugPlayer::SongFileInfo songFileInfo, const bool successfull);
    void onLoaded(const ModPlugPlayer::PlayListItem playListItem, const bool successfull);

private:
    Ui::PlayerWindow *ui;
    MppParameters * getParameters();
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
    PlayListEditorWindow *playListEditorWindow = nullptr;
    void loadSettings();
    void connectSignalsAndSlots();
    void initAndInstallEventFilters();
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void initVuMeter();
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
