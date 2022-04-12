/*
PlayerWindow class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QMainWindow>
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif
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
#include "PlayListEditor.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

class PlayerWindow : public QMainWindow, public Player
{
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     ModulePlayer modulePlayer;
	 void loadSettings();
     void setBodyColor(const RGB &backgroundColor, const RGB &textColor);

     //Player Controls
     int getVolume() const;
     bool isAlwaysOnTop() const;
     bool isSnapToViewPort() const;
     bool isKeptStayingInViewPort() const;
     bool isTitleBarHidden() const;

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
     void open(QString filePath);
     //void scrubTime(int rowId);
public slots:
    void updateTime();
    void scrubTime();
    void setTimeScrubberTicks(int amount);
    void onPreferencesWindowRequested();
    void onFileOpened();
    void onFileOpeningRequested();
    void onAboutWindowRequested();
    void onPlayListEditorWindowRequested(bool turnOn);
    void onPlayListEditorIsHidden();
    void onMinimizeRequested();
    void onMiniPlayerRequested();
    void onWindowClosingRequested();
    void onKeepStayingViewPortRequested(bool keepStayingInViewPort);
    void onChangeSnapThresholdRequested(int snappingThreshold);
    void selectNewSoundOutput(PaDeviceIndex deviceIndex);

    // Player Controls
    void onStop();
    void onPlay();
    void onPause();
    void onResume();
    void onChangeVolume(int volume);
    void onSetAlwaysOnTop(bool alwaysOnTop);
    void onHideTitleBar(bool hide);
    void onSetSnapToViewPort(bool snapToViewPort);
    void onSetSnappingThreshold(int snappingThreshold);
    void onSetKeepStayingInViewPort(bool keepStayingInViewPort);

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
    PlayListEditor *playListEditor;
    void connectSignalsAndSlots();
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void initVuMeter();
    void initMenus();
	MppParameters *parameters;
	MoveByMouseClickEventFilter *moveByMouseClick;
	KeepFixedSizeEventFilter *keepFixedSize;
    QString getSupportedExtensionsAsString();
    QString getLessKnownSupportedExtensionsAsString();
    template <typename T>
    void eraseElementFromVector(std::vector<T> &myVector, const T &value);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
	void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
};

template <typename T>
void PlayerWindow::eraseElementFromVector(std::vector<T> &elements, const T &value){
    elements.erase(std::remove(elements.begin(), elements.end(), value), elements.end());
}
