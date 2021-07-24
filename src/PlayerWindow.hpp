#ifndef PLAYERWINDOW_HPP
#define PLAYERWINDOW_HPP

#include <QMainWindow>
#ifdef Q_OS_MACOS
#include "MacManager.h"
#endif
#include <portaudiocpp/PortAudioCpp.hxx>
#include "ModulePlayerThread.hpp"
#include <QSettings>
#include <BandFilter.hpp>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QFileDialog>
#include "MppParameters.hpp"
#include "EventFilters.hpp"
#include <SpectrumAnalyzerAnimator>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

class PlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow();
     ModulePlayerThread *mpThread;
	 void loadSettings();
	 void setBodyColor(const RGB &backgroundColor, const RGB &textColor);
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
    void onMinimizeRequested();
    void onWindowClosingRequested();
	void onHideTitleBarRequested(bool hide);
private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();

    void on_volumeControl_valueChanged(int value);
    void on_stop();
    void on_play();
    void on_pause();

	void on_actionAlways_On_Top_toggled(bool arg1);

private:
    Ui::PlayerWindow *ui;
    QSettings *settings;
    SpectrumAnalyzerAnimator<double> *spectrumAnalyzerAnimator;
    SpectrumAnalyzerAnimator<double> *vuMeterAnimator;
    static portaudio::System portAudioSystem;
    QTimer *timer;
    QTimer *scrubTimer;
    QTimer *spectrumAnalyzerTimer;
    double timerTimeoutValue = 100;
    double scrubTimerTimeoutValue = 50;
    double spectrumAnalyzerTimerTimeoutValue = 40;
    bool scrubberClicked = false;
    int scrubberPreviousValue = 0;
    int scrubberClickedPosition;
    void updateSpectrumAnalyzer();
    double *spectrumData;
    QPoint dragPosition;
    QFileDialog *fileDialog;
    void connectSignalsAndSlots();
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void initVuMeter();
    void initMenus();
	MppParameters *parameters;
	MoveByMouseClickEventFilter *moveByMouseClick;
	KeepFixedSizeEventFilter *keepFixedSize;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
	void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
};
#endif // PLAYERWINDOW_HPP
