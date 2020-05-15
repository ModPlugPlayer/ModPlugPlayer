#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>


#include <cmath>
#include <cassert>
#include <cstddef>

#include <libopenmpt/libopenmpt.hpp>
#include "ModulePlayerThread.hpp"
#include "ModulePlayer.hpp"
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include "MathUtil.hpp"
#include "SpectrumAnalyzer.hpp"
#include <QMimeData>

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    setAcceptDrops(true);
    this->settings = new QSettings("ModPlug","ModPlug Player");
    portaudio::System::initialize();
    ui->setupUi(this);
    ui->menubar->hide();

    SpectrumAnalyzerParameters parameters;
    SpectrumAnalyzerParameters vuMeterParameters;
    parameters.barAmount = 20;
    spectrumData = new double[parameters.barAmount];

    std::fill(spectrumData, spectrumData + parameters.barAmount, 0);

    parameters.barDirection = Qt::Orientation::Vertical;
    /*
    parameters.barDirection = ORIENTATION::HORIZONTAL;
    parameters.barAmount = 2;
    parameters.dimmingPercentage = 30;
    parameters.transparencyPercentage = 55;
    */
    parameters.peakValue = 1;
    parameters.barGapRatio = 0.9;
    parameters.dimmingPercentage = 15;
    parameters.transparencyPercentage = 65;
    parameters.discreteParameters.ledGapRatio = 0.7;
    parameters.discreteParameters.barLedAmount = 14;

    ui->spectrumAnalyzer->setParameters(parameters);

    vuMeterParameters.barDirection = Qt::Orientation::Vertical;
    vuMeterParameters.barAmount = 1;

    vuMeterParameters.peakValue = 10;
    vuMeterParameters.barGapRatio = 0.9;
    vuMeterParameters.dimmingPercentage = 20;
    vuMeterParameters.transparencyPercentage = 65;
    vuMeterParameters.discreteParameters.ledGapRatio = 0.7;
    vuMeterParameters.discreteParameters.barLedAmount = 14;


    ui->vuMeter->setParameters(vuMeterParameters);


    this->setupWindow = new SetupWindow(this);
    this->setStyleSheet("#PlayerWindow{background-color:#c0c0c0}");
    #ifndef Q_OS_MACOS
        //ui->titleBarPlaceHolder->hide();
    #endif
    //portaudio::AutoSystem autoSys;

    mpThread = new ModulePlayerThread;

//    QObject::connect(&b, SIGNAL(b.timech(int)), this, SLOT(PlayerWindow::updateTime(int)));

//    void (PlayerControlButtons::* open)(QString) = &PlayerControlButtons::open;

    //ModulePlayerThread Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this->mpThread, &ModulePlayerThread::open);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this->mpThread, &ModulePlayerThread::stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this->mpThread, &ModulePlayerThread::pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this->mpThread, &ModulePlayerThread::play);
//    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, this->mpThread, &ModulePlayerThread::resume);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::setupClicked);

    //PlayerWindow Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::on_open);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this, &PlayerWindow::on_play);


    QObject::connect(&mpThread->mp, &ModulePlayer::timeChanged, this, &PlayerWindow::updateTime);
    QObject::connect(&mpThread->mp, &ModulePlayer::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);

    timer = new QTimer(this);
    scrubTimer = new QTimer(this);
    spectrumAnalyzerTimer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &PlayerWindow::updateTime);
    QObject::connect(scrubTimer, &QTimer::timeout, this, &PlayerWindow::scrubTime);
    QObject::connect(spectrumAnalyzerTimer, &QTimer::timeout, this, &PlayerWindow::updateSpectrumAnalyzer);
    timer->start(timerTimeoutValue);

    mpThread->start();

    ui->centralwidget->installEventFilter(this);

    //mp.play();
    //portaudio::System::instance().sleep(NUM_SECONDS*1000);

    //portAudioSystem.sleep(NUM_SECONDS * 1000);

//    mp.stream.stop();

//    mp.stream.close();


    //mp.play();
    //mp.stream.start();

    //mp.play();
    //MPP mpp;
    //mpp.play();
    //BeeperWIthCallback b;
    //b.open(this->portAudioSystem);
}

void PlayerWindow::updateTime() {
    TimeInfo timeInfo = mpThread->mp.getTimeInfo();
    ui->timeScrubber->setValue(timeInfo.globalRowIndex);
    ui->lcdPanel->updateTime(mpThread->mp.getTimeInfo().seconds);
    updateSpectrumAnalyzer();
}

void PlayerWindow::setTimeScrubberTicks(int amount) {
    if(amount == 0)
        amount = 1;
    ui->timeScrubber->setMaximum((amount-1));
}

void PlayerWindow::setupClicked()
{
    setupWindow->show();
}

PlayerWindow::~PlayerWindow()
{
    portaudio::System::terminate();
    delete this->setupWindow;
    delete ui;
}

void PlayerWindow::on_timeScrubber_sliderMoved(int position)
{
    scrubberClickedPosition = position;
    //mp->mp.scrubTime(position);
}

void PlayerWindow::scrubTime(){
    if(scrubberClickedPosition != scrubberPreviousValue)
        mpThread->mp.scrubTime(scrubberClickedPosition);
    scrubberPreviousValue = scrubberClickedPosition;
}

void PlayerWindow::on_timeScrubber_sliderPressed()
{
    timer->stop();
    scrubberClickedPosition = ui->timeScrubber->value();
    scrubTimer->start(scrubTimerTimeoutValue);
    mpThread->mp.scrubTime(scrubberClickedPosition);
}

void PlayerWindow::on_timeScrubber_sliderReleased()
{
//    updateTime();
    scrubTimer->stop();
    timer->start(timerTimeoutValue);
}

void PlayerWindow::updateSpectrumAnalyzer()
{
    mpThread->mp.getSpectrumData(spectrumData);
    float vuMeterDbValue = mpThread->mp.getVuMeterValue() + 20;
    ui->vuMeter->setBarValue(0, vuMeterDbValue);
    for(int i=0; i<20; i++) {
        //qDebug()<<spectrumData[i].magnitude/spectrumData[i].sampleAmount;
        double barValue = spectrumData[i];
        //qDebug()<<"barValue:"<<spectrumData[i].magnitude;
        //barValue = DSP::calculateMagnitudeDb(barValue);
        ui->spectrumAnalyzer->setBarValue(i, barValue/200);

        /*
        double val = MathUtil::clamp<double>(spectrumData[i], -50, 0);
        val += 50;
        val *= 2;
        */
    }

    ui->spectrumAnalyzer->update();
    ui->vuMeter->update();

    /*
    ui->progressBar_1->setValue(MathUtil::clamp<double>(spectrumData[0], -50, 0));
    ui->progressBar_2->setValue(MathUtil::clamp<double>(spectrumData[1], -50, 0));
    ui->progressBar_3->setValue(MathUtil::clamp<double>(spectrumData[2], -50, 0));
    ui->progressBar_4->setValue(MathUtil::clamp<double>(spectrumData[3], -50, 0));
    ui->progressBar_5->setValue(MathUtil::clamp<double>(spectrumData[4], -50, 0));
    ui->progressBar_6->setValue(MathUtil::clamp<double>(spectrumData[5], -50, 0));
    ui->progressBar_7->setValue(MathUtil::clamp<double>(spectrumData[6], -50, 0));
    ui->progressBar_8->setValue(MathUtil::clamp<double>(spectrumData[7], -50, 0));
    ui->progressBar_9->setValue(MathUtil::clamp<double>(spectrumData[8], -50, 0));
    ui->progressBar_10->setValue(MathUtil::clamp<double>(spectrumData[9], -50, 0));
    ui->progressBar_11->setValue(MathUtil::clamp<double>(spectrumData[10], -50, 0));
    ui->progressBar_12->setValue(MathUtil::clamp<double>(spectrumData[11], -50, 0));
*/
}

void PlayerWindow::on_volumeControl_valueChanged(int value)
{
    double linearVolume = ((double)value)/100.0f;
    double exponentialVolume = DSP<double>::calculateExponetialVolume(linearVolume);
    mpThread->mp.setVolume(exponentialVolume);
    qDebug()<<"Linear Volume: "<<linearVolume;
    qDebug()<<"Exponential Volume "<<exponentialVolume;
}

void PlayerWindow::on_open(QString filePath)
{
    QString title = QString::fromStdString(mpThread->mp.getSongTitle());
    ui->lcdPanel->setSongTitle(title);
}

void PlayerWindow::on_stop()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->stop();
    qDebug()<<"Stop";
}

void PlayerWindow::on_play()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->start(spectrumAnalyzerTimerTimeoutValue);
    qDebug()<<"Play";
}
void PlayerWindow::on_pause()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Pause";
}

void PlayerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void PlayerWindow::dropEvent(QDropEvent *event)
{
    emit ui->playerControlButtons->open(event->mimeData()->urls()[0].toLocalFile());
    emit ui->playerControlButtons->play();
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

bool PlayerWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->timeScrubber) {
        event->accept();
        return false;
    }
    if (watched == ui->centralwidget)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
            if (mouse_event->button() == Qt::LeftButton)
            {
                dragPosition = mouse_event->globalPos() - frameGeometry().topLeft();
                return false;
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
            if (mouse_event->buttons() & Qt::LeftButton)
            {
                move(mouse_event->globalPos() - dragPosition);
                return false;
            }
        }

    }
    return false;
}
