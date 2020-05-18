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
#include <DSP.hpp>
#include "AboutWindow.hpp"

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    setAcceptDrops(true);
    this->settings = new QSettings("ModPlug","ModPlug Player");
    portaudio::System::initialize();
    ui->setupUi(this);


    initMenus();
    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));

    initSpectrumAnalyzer();

    initVuMeter();

    this->setupWindow = new SetupWindow(this);
    this->setStyleSheet("#PlayerWindow{background-color:#c0c0c0}");
    #ifndef Q_OS_MACOS
        //ui->titleBarPlaceHolder->hide();
    #endif
    //portaudio::AutoSystem autoSys;

    mpThread = new ModulePlayerThread;

//    QObject::connect(&b, SIGNAL(b.timech(int)), this, SLOT(PlayerWindow::updateTime(int)));

//    void (PlayerControlButtons::* open)(QString) = &PlayerControlButtons::open;

    connectSignalsAndSlots();

    initAndConnectTimers();

    mpThread->start();

    ui->centralwidget->installEventFilter(this);

    QVariant v = settings->value("Volume", 50);

    ui->volumeControl->setValue(v.toInt());

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

void PlayerWindow::onPreferencesWindowRequested() {
    setupWindow->show();
}

PlayerWindow::~PlayerWindow()
{
    QVariant vol;
    vol.setValue<int>(ui->volumeControl->value());
    settings->setValue("Volume", vol);

    portaudio::System::terminate();
    delete fileDialog;
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
    float volumeCoefficient = double(ui->volumeControl->value())/100;
    float vuMeterDbValue = mpThread->mp.getVuMeterValue();
    //qDebug()<<"vu:"<<vuMeterDbValue;
    ui->vuMeter->setBarValue(0, vuMeterDbValue);
    for(int i=0; i<20; i++) {
        //qDebug()<<spectrumData[i].magnitude/spectrumData[i].sampleAmount;
        double barValue = spectrumData[i];
        //qDebug()<<"barValue["<<i<<"]:"<<barValue;
        //qDebug()<<"barValue:"<<spectrumData[i].magnitude;
        //barValue = DSP::calculateMagnitudeDb(barValue);
        ui->spectrumAnalyzer->setBarValue(i, barValue);

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

void PlayerWindow::connectSignalsAndSlots()
{
    //ModulePlayerThread Connections
    QObject::connect(this, &PlayerWindow::open, this->mpThread, &ModulePlayerThread::open);
    QObject::connect(this->mpThread, &ModulePlayerThread::fileOpened, this, &PlayerWindow::onFileOpened);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this->mpThread, &ModulePlayerThread::stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this->mpThread, &ModulePlayerThread::pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this->mpThread, &ModulePlayerThread::play);
//    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, this->mpThread, &ModulePlayerThread::resume);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::onPreferencesWindowRequested);
    QObject::connect(this->ui->optionButtons, &OptionButtons::about, this, &PlayerWindow::onAboutWindowRequested);

    //PlayerWindow Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::onFileOpeningRequested);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this, &PlayerWindow::on_play);


    QObject::connect(&mpThread->mp, &ModulePlayer::timeChanged, this, &PlayerWindow::updateTime);
    QObject::connect(&mpThread->mp, &ModulePlayer::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);

    //Menu Items
    QObject::connect(this->ui->actionOpen, &QAction::triggered, this, &PlayerWindow::onFileOpeningRequested);
    QObject::connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, this, &PlayerWindow::onAboutWindowRequested);
    QObject::connect(this->ui->actionPreferences, &QAction::triggered, this, &PlayerWindow::onPreferencesWindowRequested);
    QObject::connect(this->ui->actionPlay, &QAction::triggered, this->mpThread, &ModulePlayerThread::play);
    QObject::connect(this->ui->actionPause, &QAction::triggered, this->mpThread, &ModulePlayerThread::pause);
    QObject::connect(this->ui->actionStop, &QAction::triggered, this->mpThread, &ModulePlayerThread::stop);
    QObject::connect(this->ui->actionPlay, &QAction::triggered, this, &PlayerWindow::on_play);
    QObject::connect(this->ui->actionPause, &QAction::triggered, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->actionStop, &QAction::triggered, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->actionMinimize, &QAction::triggered, this, &PlayerWindow::onMinimizeRequested);
    QObject::connect(this->ui->actionCloseApp, &QAction::triggered, this, &PlayerWindow::onWindowClosingRequested);

}

void PlayerWindow::initAndConnectTimers()
{
    timer = new QTimer(this);
    scrubTimer = new QTimer(this);
    spectrumAnalyzerTimer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &PlayerWindow::updateTime);
    QObject::connect(scrubTimer, &QTimer::timeout, this, &PlayerWindow::scrubTime);
    QObject::connect(spectrumAnalyzerTimer, &QTimer::timeout, this, &PlayerWindow::updateSpectrumAnalyzer);
    timer->start(timerTimeoutValue);
}

void PlayerWindow::initSpectrumAnalyzer()
{
    SpectrumAnalyzerParameters parameters;
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
    parameters.peakValue = 100;
    parameters.barGapRatio = 0.9;
    parameters.dimmingPercentage = 15;
    parameters.transparencyPercentage = 65;
    parameters.discreteParameters.ledGapRatio = 0.7;
    parameters.discreteParameters.barLedAmount = 14;

    ui->spectrumAnalyzer->setParameters(parameters);
}

void PlayerWindow::initVuMeter()
{
    SpectrumAnalyzerParameters vuMeterParameters;

    vuMeterParameters.barDirection = Qt::Orientation::Vertical;
    vuMeterParameters.barAmount = 1;

    vuMeterParameters.peakValue = -8;
    vuMeterParameters.floorValue = -40;
    vuMeterParameters.barGapRatio = 0.9;
    vuMeterParameters.dimmingPercentage = 20;
    vuMeterParameters.transparencyPercentage = 65;
    vuMeterParameters.discreteParameters.ledGapRatio = 0.7;
    vuMeterParameters.discreteParameters.barLedAmount = 14;

    ui->vuMeter->setParameters(vuMeterParameters);
}

void PlayerWindow::initMenus()
{
    ui->menubar->hide();

    ui->actionAbout_ModPlug_Player->setMenuRole(QAction::ApplicationSpecificRole);
    QAction * aboutSeparator = ui->menuFile->addSeparator();
    aboutSeparator->setMenuRole(QAction::ApplicationSpecificRole);

    ui->actionPreferences->setMenuRole(QAction::ApplicationSpecificRole);
}

void PlayerWindow::on_volumeControl_valueChanged(int value) {
    double linearVolume = ((double)value)/100.0f;
    double exponentialVolume = DSP<double>::calculateExponetialVolume(linearVolume);
    mpThread->mp.setVolume(exponentialVolume);
    //qDebug()<<"Linear Volume: "<<linearVolume;
    //qDebug()<<"Exponential Volume "<<exponentialVolume;
}

void PlayerWindow::onFileOpened() {
    QString title = QString::fromStdString(mpThread->mp.getSongTitle());
    ui->lcdPanel->setSongTitle(title);
    size_t duration = mpThread->mp.getSongDuration();
    ui->lcdPanel->setSongDuration(duration);
}

void PlayerWindow::onFileOpeningRequested(){
    QString filePath;
    filePath = fileDialog->getOpenFileName(this, "Open Module File", QString(), tr("All Modules (*.mod mod.* *.xm *.it *.s3m)"));
    if (!filePath.isEmpty()){
        emit(open(filePath));
    }
}

void PlayerWindow::onAboutWindowRequested() {
    AboutWindow aboutWindow(this);
//    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void PlayerWindow::onMinimizeRequested()
{
    showMinimized();
}

void PlayerWindow::onWindowClosingRequested()
{
    hide();
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
    emit open(event->mimeData()->urls()[0].toLocalFile());
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
