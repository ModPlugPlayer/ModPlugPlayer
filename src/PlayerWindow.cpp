/*
PlayerWindow class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <QDebug>


#include <cmath>
#include <cassert>
#include <cstddef>

#include <libopenmpt/libopenmpt.hpp>
#include "ModulePlayer.hpp"
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include "MathUtil.hpp"
#include <SpectrumAnalyzer.hpp>
#include <QMimeData>
#include <DSP.hpp>
#include "AboutWindow.hpp"
#include <QCloseEvent>
#include "SetupWindow.hpp"

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);
    #ifdef Q_OS_MACOS
        ui->frame->setStyleSheet("");
    #else
        setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
        ui->titleBar->setMinimumHeight(14);
        ui->titleBar->setMaximumHeight(14);
    #endif

    QMainWindow::setWindowIcon(QIcon(":/MPP.iconset/icon_512x512.png"));

	setAcceptDrops(true);

	this->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "ModPlug", "ModPlug Player");
	this->parameters = new MppParameters(settings);
	parameters->load();

	qDebug()<<settings->fileName();

    portaudio::System::initialize();


    initMenus();
    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));

    this->spectrumAnalyzerAnimator = new SpectrumAnalyzerAnimator<double>(20, 0, 100);
    this->vuMeterAnimator = new SpectrumAnalyzerAnimator<double>(1, -40, -8);

    MotionProperties rs, fs, rv, fv;
    rs.acceleration = -10000;
    rs.motionType = MotionType::ConstantAcceleration;

    fs.acceleration = -10000;
    fs.motionType = MotionType::ConstantAcceleration;

    rv.acceleration = -2500;
    fv.acceleration = -2500;
    rv.motionType = MotionType::ConstantAcceleration;
    fv.motionType = MotionType::ConstantAcceleration;


    spectrumAnalyzerAnimator->setFallingMotionProperties(fs);
    spectrumAnalyzerAnimator->setRaisingMotionProperties(rs);
    spectrumAnalyzerAnimator->start();

    vuMeterAnimator->setFallingMotionProperties(fv);
    vuMeterAnimator->setRaisingMotionProperties(rv);
    vuMeterAnimator->start();



    initSpectrumAnalyzer();

    initVuMeter();

    #ifndef Q_OS_MACOS
        //ui->titleBarPlaceHolder->hide();
    #endif
    //portaudio::AutoSystem autoSys;

//    QObject::connect(&b, SIGNAL(b.timech(int)), this, SLOT(PlayerWindow::updateTime(int)));

//    void (PlayerControlButtons::* open)(QString) = &PlayerControlButtons::open;

    connectSignalsAndSlots();

    initAndConnectTimers();

	moveByMouseClick = new MoveByMouseClickEventFilter(this);
	keepFixedSize = new KeepFixedSizeEventFilter(this);

    ui->centralwidget->installEventFilter(this);
	ui->lcdPanel->installEventFilter(moveByMouseClick);
	ui->spectrumAnalyzerFrame->installEventFilter(moveByMouseClick);
	ui->vuMeterFrame->installEventFilter(moveByMouseClick);
	ui->centralwidget->installEventFilter(keepFixedSize);

	ui->centralwidget->setMouseTracking(true);

	int volume = parameters->volume;
	ui->volumeControl->setValue(volume);

	loadSettings();

	ui->timeScrubber->setEnabled(false);

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

void PlayerWindow::loadSettings() {
	ui->titleBar->setActiveColor(parameters->activeTitlebarTextColor);
	ui->titleBar->setInactiveColor(parameters->inactiveTitlebarTextColor);
	setBodyColor(parameters->playerBodyBackgroundColor, parameters->playerBodyTextColor);
    ui->playerControlButtons->setActiveButtonLightColor(parameters->activeButtonLightColor);
    ui->playerControlButtons->setInactiveButtonLightColor(parameters->inactiveButtonLightColor);
    ui->playerControlButtons->setBackgroundColor(parameters->playerBodyBackgroundColor);
    ui->playerControlButtons->setTextColor(parameters->playerBodyTextColor);
    ui->optionButtons->setActiveButtonLightColor(parameters->activeButtonLightColor);
    ui->optionButtons->setInactiveButtonLightColor(parameters->inactiveButtonLightColor);
    ui->optionButtons->setBackgroundColor(parameters->playerBodyBackgroundColor);
    ui->optionButtons->setTextColor(parameters->playerBodyTextColor);
    ui->lcdPanel->setBackgroundColor(parameters->lcdDisplayBackgroundColor);
    ui->lcdPanel->setTextColor(parameters->lcdDisplayForegroundColor);
}

void PlayerWindow::setBodyColor(const RGB &backgroundColor, const RGB &textColor){
    QString style = QString("#PlayerWindow{background-color:%1;}; #PlayerControlButtons{color:%2;}").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    this->setStyleSheet(style);
    ui->timeScrubber->setStyleSheet(QString(".QSlider::handle:horizontal {background-color:%1;}").arg(backgroundColor.hex().c_str()));
    ui->volumeControl->setStyleSheet(QString(".QSlider::handle:vertical {background-color:%1;}").arg(backgroundColor.hex().c_str()));
}

void PlayerWindow::updateTime() {
    TimeInfo timeInfo = modulePlayer.getTimeInfo();
    ui->timeScrubber->setValue(timeInfo.globalRowIndex);
    ui->lcdPanel->updateTime(modulePlayer.getTimeInfo().seconds);
    updateSpectrumAnalyzer();
}

void PlayerWindow::setTimeScrubberTicks(int amount) {
    if(amount == 0)
        amount = 1;
    ui->timeScrubber->setMaximum((amount-1));
}

void PlayerWindow::onPreferencesWindowRequested() {
	SetupWindow setupWindow(parameters, this);
	setupWindow.exec();
}

PlayerWindow::~PlayerWindow()
{
	//QVariant vol;
	//vol.setValue<int>(ui->volumeControl->value());
	//settings->setValue("Volume", ui->volumeControl->value());
	parameters->volume = ui->volumeControl->value();
	parameters->save();

    portaudio::System::terminate();
    delete fileDialog;
    delete ui;
}

void PlayerWindow::on_timeScrubber_sliderMoved(int position)
{
    scrubberClickedPosition = position;
    //mp->mp.scrubTime(position);
}

void PlayerWindow::scrubTime(){
    if(modulePlayer.isSongState(SongState::Loaded)) {
		if(scrubberClickedPosition != scrubberPreviousValue)
            modulePlayer.scrubTime(scrubberClickedPosition);
		scrubberPreviousValue = scrubberClickedPosition;
	}
}

void PlayerWindow::on_timeScrubber_sliderPressed()
{
    if(modulePlayer.isSongState(SongState::Loaded)) {
		timer->stop();
		scrubberClickedPosition = ui->timeScrubber->value();
		scrubTimer->start(scrubTimerTimeoutValue);
        modulePlayer.scrubTime(scrubberClickedPosition);
	}
}
void PlayerWindow::on_timeScrubber_sliderReleased()
{
    if(modulePlayer.isSongState(SongState::Loaded)) {
	//    updateTime();
		scrubTimer->stop();
		timer->start(timerTimeoutValue);
	}
}

void PlayerWindow::updateSpectrumAnalyzer()
{
    modulePlayer.getSpectrumData(spectrumData);
    spectrumAnalyzerAnimator->setValues(spectrumData);
    spectrumAnalyzerAnimator->getValues(spectrumData);
    float volumeCoefficient = double(ui->volumeControl->value())/100;
    double vuMeterDbValue = modulePlayer.getVuMeterValue();
    if(vuMeterDbValue == NAN)
        vuMeterDbValue = 0;
    else if(vuMeterDbValue < -40)
        vuMeterDbValue = -40;
    else if(vuMeterDbValue > -8)
        vuMeterDbValue = -8;
    vuMeterAnimator->setValues(&vuMeterDbValue);
    vuMeterAnimator->getValues(&vuMeterDbValue);
    ui->vuMeter->setBarValue(0, vuMeterDbValue);
    for(int i=0; i<20; i++) {
        //qDebug()<<spectrumData[i].magnitude/spectrumData[i].sampleAmount;
        double barValue = spectrumData[i];
        if(barValue == NAN)
            barValue = 0;
        else if(barValue < 0)
            barValue = 0;
        else if(barValue > 100)
            barValue = 100;

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
    QObject::connect(this, &PlayerWindow::open, &this->modulePlayer, &ModulePlayer::open);
    QObject::connect(&this->modulePlayer, &ModulePlayer::fileOpened, this, &PlayerWindow::onFileOpened);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, &modulePlayer, &ModulePlayer::stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, &modulePlayer, &ModulePlayer::pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, &modulePlayer, &ModulePlayer::play);
//    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, &modulePlayer, &ModulePlayer::resume);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::onPreferencesWindowRequested);
    QObject::connect(this->ui->optionButtons, &OptionButtons::about, this, &PlayerWindow::onAboutWindowRequested);

    //PlayerWindow Connections
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::onFileOpeningRequested);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->playerControlButtons, &PlayerControlButtons::play, this, &PlayerWindow::on_play);


    QObject::connect(&modulePlayer, &ModulePlayer::timeChanged, this, &PlayerWindow::updateTime);
    QObject::connect(&modulePlayer, &ModulePlayer::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);

    //Menu Items
    QObject::connect(this->ui->actionOpen, &QAction::triggered, this, &PlayerWindow::onFileOpeningRequested);
    QObject::connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, this, &PlayerWindow::onAboutWindowRequested);
    QObject::connect(this->ui->actionPreferences, &QAction::triggered, this, &PlayerWindow::onPreferencesWindowRequested);
    QObject::connect(this->ui->actionPlay, &QAction::triggered, &modulePlayer, &ModulePlayer::play);
    QObject::connect(this->ui->actionPause, &QAction::triggered, &modulePlayer, &ModulePlayer::pause);
    QObject::connect(this->ui->actionStop, &QAction::triggered, &modulePlayer, &ModulePlayer::stop);
    QObject::connect(this->ui->actionPlay, &QAction::triggered, this, &PlayerWindow::on_play);
    QObject::connect(this->ui->actionPause, &QAction::triggered, this, &PlayerWindow::on_pause);
    QObject::connect(this->ui->actionStop, &QAction::triggered, this, &PlayerWindow::on_stop);
    QObject::connect(this->ui->actionMinimize, &QAction::triggered, this, &PlayerWindow::onMinimizeRequested);
    QObject::connect(this->ui->actionCloseApp, &QAction::triggered, this, &PlayerWindow::onWindowClosingRequested);

    QObject::connect(this->ui->titleBar, &TitleBar::minimizeButtonClicked, this, &PlayerWindow::onMinimizeRequested);
    QObject::connect(this->ui->titleBar, &TitleBar::miniPlayerButtonClicked, this, &PlayerWindow::onMiniPlayerRequested);
    QObject::connect(this->ui->titleBar, &TitleBar::closeButtonClicked, this, &PlayerWindow::onWindowClosingRequested);

    QObject::connect(&modulePlayer, &ModulePlayer::playerStateChanged, ui->playerControlButtons, &PlayerControlButtons::on_playerState_changed);
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
    parameters.discreteParameters.barLedAmount = this->parameters->spectrumAnalyzerLedAmount;
    parameters.barType = BarType::Continuous;
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
    modulePlayer.setVolume(exponentialVolume);
    //qDebug()<<"Linear Volume: "<<linearVolume;
    //qDebug()<<"Exponential Volume "<<exponentialVolume;
}

void PlayerWindow::onFileOpened() {
    std::string songTitle = modulePlayer.getSongTitle();
    QString title = QString::fromUtf8(songTitle);
    if(title.trimmed().isEmpty())
        title = QString::fromStdString(modulePlayer.getFilePath().stem().string());
    ui->lcdPanel->setSongTitle(title);
    ui->titleBar->setTitle(QString("ModPlug Player - ") + QString::fromStdString(modulePlayer.getFilePath().filename().string()));
    size_t duration = modulePlayer.getSongDuration();
    ui->lcdPanel->setSongDuration(duration);
	ui->timeScrubber->setEnabled(true);
}

void PlayerWindow::onFileOpeningRequested(){
    modulePlayer.stop();
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

void PlayerWindow::onMiniPlayerRequested()
{

}

void PlayerWindow::onWindowClosingRequested()
{
	hide();
}

void PlayerWindow::onHideTitleBarRequested(bool hide)
{
	if(hide) {
		ui->titleBar->hide();
		ui->centralwidget->layout()->setContentsMargins(2,2,2,2);
	}
	else {
		ui->titleBar->show();
		ui->centralwidget->layout()->setContentsMargins(2,0,2,2);
	}
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
    modulePlayer.stop();
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

    return false;
}

void PlayerWindow::closeEvent (QCloseEvent *event) {
    if(parameters->hideByCloseButton) {
        hide();
        event->ignore();
    }
    else {
        event->accept();
    }
    //
}

void PlayerWindow::on_actionAlways_On_Top_toggled(bool alwaysOnTop) {
	#ifdef Q_OS_MACOS
		MacManager::toggleAlwaysOnTop(this->winId(), alwaysOnTop);
    #elif defined(Q_OS_WIN)
		// #include <windows.h>
		if (alwaysOnTop)
		{
            this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		}
		else
		{
            this->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
        }
	#else
		Qt::WindowFlags flags = this->windowFlags();
		if (alwaysOnTop)
		{
			this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
			this->show();
		}
		else
		{
			this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
			this->show();
		}
	#endif
}
