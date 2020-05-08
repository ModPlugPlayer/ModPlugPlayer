#ifndef PLAYERWINDOW_HPP
#define PLAYERWINDOW_HPP

#include <QMainWindow>
#include <portaudiocpp/PortAudioCpp.hxx>
#include "ModulePlayerThread.hpp"
#include <QSettings>
#include <SetupWindow.hpp>

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
//     static PLAYERSTATE playerState;
//     static SONGSTATE songState;
signals:
     //void scrubTime(int rowId);
public slots:
    void updateTime();
    void scrubTime();
    void setTimeScrubberTicks(int amount);
    void setupClicked();
private slots:
    void on_timeScrubber_sliderMoved(int position);
    void on_timeScrubber_sliderPressed();
    void on_timeScrubber_sliderReleased();

    void on_volumeControl_valueChanged(int value);
    void on_open(QString filePath);
    void on_stop();
    void on_play();
    void on_pause();

private:
    Ui::PlayerWindow *ui;
    QSettings *settings;
    static portaudio::System portAudioSystem;
    QTimer *timer;
    QTimer *scrubTimer;
    QTimer *spectrumAnalyzerTimer;
    double timerTimeoutValue = 100;
    double scrubTimerTimeoutValue = 50;
    bool scrubberClicked = false;
    int scrubberPreviousValue = 0;
    int scrubberClickedPosition;
    double getExponentialVolume(double &linearVolume);
    SetupWindow *setupWindow;
    void updateSpectrumAnalyzer();
    std::vector<double> spectrumData;
};
#endif // PLAYERWINDOW_HPP
