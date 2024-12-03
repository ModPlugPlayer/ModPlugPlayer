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
#include "Util/WindowUtil.hpp"
#include <QOverload>

PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayerWindow)
{
    ui->setupUi(this);

    #ifdef Q_OS_MACOS
        setUnifiedTitleAndToolBarOnMac(true);

        //Makes the components seem under the transparent titlebar correctly
        setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);
        ui->centralwidget->setAttribute(Qt::WA_ContentsMarginsRespectsSafeArea, false);

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

    playListEditorWindow = new PlayListEditorWindow(this, this);

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

//    connect(&b, SIGNAL(b.timech(int)), this, SLOT(PlayerWindow::updateTime(int)));

//    void (PlayerControlButtons::* open)(QString) = &PlayerControlButtons::open;

    connectSignalsAndSlots();

    initAndConnectTimers();

	moveByMouseClick = new MoveByMouseClickEventFilter(this);

    ui->centralwidget->installEventFilter(this);
	ui->lcdPanel->installEventFilter(moveByMouseClick);
	ui->spectrumAnalyzerFrame->installEventFilter(moveByMouseClick);
	ui->vuMeterFrame->installEventFilter(moveByMouseClick);
    //ui->centralwidget->installEventFilter(keepFixedSize);
    ui->titleBar->installEventFilter(moveByMouseClick);

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
    modulePlayer.setOutputDeviceIndex(parameters->audioDeviceIndex);
    onKeepingStayingInViewPortStateChangeRequested(parameters->keepStayingInViewPort);
    onSnappingToViewPortStateChangeRequested(parameters->snapToViewPort);
    onSnappingThresholdChangeRequested(parameters->snappingThreshold);
    onAlwaysOnTopStateChangeRequested(parameters->alwaysOnTop);
    onTitleBarHidingStateChangeRequested(parameters->hideTitleBar);
    moveByMouseClick->setSnappingThreshold(parameters->snappingThreshold);
    setSpectrumAnalyzerWindowFunction(parameters->spectrumAnalyzerWindowFunction);
    resize(parameters->playerWindowSize);
}

void PlayerWindow::setBodyColor(const RGB &backgroundColor, const RGB &textColor){
    QString style = QString("#PlayerWindow{background-color:%1;}; #PlayerControlButtons{color:%2;}").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    this->setStyleSheet(style);
    ui->centralwidget->setStyleSheet(QString("background-color:%1;").arg(backgroundColor.hex().c_str()));
    ui->timeScrubber->setStyleSheet(QString(".QSlider::handle:horizontal {background-color:%1;}").arg(backgroundColor.hex().c_str()));
    ui->volumeControl->setStyleSheet(QString(".QSlider::handle:vertical {background-color:%1;}").arg(backgroundColor.hex().c_str()));
}

int PlayerWindow::getVolume() const
{
    return parameters->volume;
}

bool PlayerWindow::isAlwaysOnTop() const
{
    return parameters->alwaysOnTop;
}

bool PlayerWindow::isSnapToViewPort() const
{
    return parameters->snapToViewPort;
}

bool PlayerWindow::isKeptStayingInViewPort() const
{
    return parameters->keepStayingInViewPort;
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
    parameters->save();
    bool stateAlwaysOnTop = isAlwaysOnTop();
    WindowUtil::setAlwaysOnTop(this, false);
    SetupWindow setupWindow(parameters, this);
	setupWindow.exec();
    WindowUtil::setAlwaysOnTop(this, stateAlwaysOnTop);
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

void PlayerWindow::updateTimeScrubber(){
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
        vuMeterDbValue = parameters->vuMeterMinimumValue;
    else if(vuMeterDbValue < parameters->vuMeterMinimumValue)
        vuMeterDbValue = parameters->vuMeterMinimumValue;
    else if(vuMeterDbValue > parameters->vuMeterMaximumValue)
        vuMeterDbValue = parameters->vuMeterMaximumValue;
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
        else if(barValue > parameters->spectrumAnalyzerMaximumValue)
            barValue = parameters->spectrumAnalyzerMaximumValue;

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
    connect(this, qOverload<std::filesystem::path>(&PlayerWindow::openRequested), &this->modulePlayer, qOverload<std::filesystem::path>(&ModulePlayer::load));
    connect(this, qOverload<PlayListItem>(&PlayerWindow::openRequested), &this->modulePlayer, qOverload<PlayListItem>(&ModulePlayer::load));
    connect(&this->modulePlayer, &ModulePlayer::moduleFileLoaded, this, &PlayerWindow::onFileLoaded);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::stop, &modulePlayer, &ModulePlayer::stop);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::pause, &modulePlayer, &ModulePlayer::pause);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::play, &modulePlayer, &ModulePlayer::play);
//    connect(this->ui->playerControlButtons, &PlayerControlButtons::fastForward, &modulePlayer, &ModulePlayer::resume);
    connect(this->ui->playerControlButtons, &PlayerControlButtons::setup, this, &PlayerWindow::onPreferencesWindowRequested);

    connect(this->ui->optionButtons, &OptionButtons::about, this, &PlayerWindow::onAboutWindowRequested);
    connect(this->ui->optionButtons, &OptionButtons::playlist, this, &PlayerWindow::onPlayListEditorWindowRequested);
    connect(this->ui->optionButtons, &OptionButtons::repeat, this, &PlayerWindow::onRepeatModeToggleRequested);

    connect(this->playListEditorWindow, &PlayListEditorWindow::hidden, this, &PlayerWindow::onPlayListEditorIsHidden);


    //PlayerWindow Connections
    connect(this->ui->playerControlButtons, &PlayerControlButtons::open, this, &PlayerWindow::onFileOpeningRequested);
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::stop), this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::pause), this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(this->ui->playerControlButtons, qOverload<>(&PlayerControlButtons::play), this, qOverload<>(&PlayerWindow::onPlayRequested));

    connect(this, &PlayerWindow::repeatModeChangeRequested, this, &PlayerWindow::onRepeatModeChangeRequested);

    connect(&modulePlayer, &ModulePlayer::timeChanged, this, &PlayerWindow::updateTime);
    connect(&modulePlayer, &ModulePlayer::timeTicksAmountChanged, this, &PlayerWindow::setTimeScrubberTicks);

    //Menu Items
    connect(this->ui->actionOpen, &QAction::triggered, this, &PlayerWindow::onFileOpeningRequested);
    connect(this->ui->actionAbout_ModPlug_Player, &QAction::triggered, this, &PlayerWindow::onAboutWindowRequested);
    connect(this->ui->actionPreferences, &QAction::triggered, this, &PlayerWindow::onPreferencesWindowRequested);
    connect(this->ui->actionPlay, &QAction::triggered, &modulePlayer, &ModulePlayer::play);
    connect(this->ui->actionPause, &QAction::triggered, &modulePlayer, &ModulePlayer::pause);
    connect(this->ui->actionStop, &QAction::triggered, &modulePlayer, &ModulePlayer::stop);
    connect(this->ui->actionPlay, &QAction::triggered, this, qOverload<>(&PlayerWindow::onPlayRequested));
    connect(this->ui->actionPause, &QAction::triggered, this, qOverload<>(&PlayerWindow::onPauseRequested));
    connect(this->ui->actionStop, &QAction::triggered, this, qOverload<>(&PlayerWindow::onStopRequested));
    connect(this->ui->actionMinimize, &QAction::triggered, this, &PlayerWindow::onMinimizeRequested);
    connect(this->ui->actionPlayListEditor, &QAction::toggled, this, &PlayerWindow::onPlayListEditorWindowRequested);
    connect(this->ui->actionAlways_On_Top, &QAction::toggled, this, &PlayerWindow::onAlwaysOnTopStateChangeRequested);
    connect(this->ui->actionHideTitleBar, &QAction::toggled, this, &PlayerWindow::onTitleBarHidingStateChangeRequested);
    connect(this->ui->actionSnap_to_Viewport, &QAction::toggled, this, &PlayerWindow::onSnappingToViewPortStateChangeRequested);
    connect(this->ui->actionKeep_Staying_in_ViewPort, &QAction::toggled, this, &PlayerWindow::onKeepingStayingInViewPortStateChangeRequested);

    connect(this->ui->volumeControl, &QSlider::valueChanged, this, &PlayerWindow::onVolumeChangeRequested);

    connect(this->ui->titleBar, &TitleBar::minimizeButtonClicked, this, &PlayerWindow::onMinimizeRequested);
    connect(this->ui->titleBar, &TitleBar::miniPlayerButtonClicked, this, &PlayerWindow::onMiniPlayerRequested);
    connect(this->ui->titleBar, &TitleBar::closeButtonClicked, this, &PlayerWindow::onWindowClosingRequested);

    connect(&modulePlayer, &ModulePlayer::playerStateChanged, ui->playerControlButtons, &PlayerControlButtons::on_playerState_changed);

    //LCD Display Properties Area Connections
    connect(this, &PlayerWindow::repeatModeChanged, ui->lcdPanel, &LCDDisplay::onRepeatModeChanged);
    connect(this, &PlayerWindow::eqStateChanged, ui->lcdPanel, &LCDDisplay::onEqStateChanged);
    connect(this, &PlayerWindow::agcStateChanged, ui->lcdPanel, &LCDDisplay::onAGCStateChanged);
    connect(this, &PlayerWindow::xBassStateChanged, ui->lcdPanel, &LCDDisplay::onXBassStateChanged);
    connect(this, &PlayerWindow::surroundStateChanged, ui->lcdPanel, &LCDDisplay::onSurroundStateChanged);
    connect(this, &PlayerWindow::reverbStateChanged, ui->lcdPanel, &LCDDisplay::onReverbStateChanged);
    connect(this, &PlayerWindow::interpolationFilterChanged, ui->lcdPanel, &LCDDisplay::onInterpolationFilterChanged);

    connect(ui->lcdPanel, &LCDDisplay::repeatModeChangeRequested, this, &PlayerWindow::repeatModeChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::eqStateChangeRequested, this, &PlayerWindow::onEqStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::agcStateChangeRequested, this, &PlayerWindow::onAGCStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::xBassStateChangeRequested, this, &PlayerWindow::onXBassStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::surroundStateChangeRequested, this, &PlayerWindow::onSurroundStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::reverbStateChangeRequested, this, &PlayerWindow::onReverbStateChangeRequested);
    connect(ui->lcdPanel, &LCDDisplay::interpolationFilterChangeRequested, this, &PlayerWindow::onInterpolationFilterChangeRequested);


}

void PlayerWindow::initAndConnectTimers()
{
    timer = new QTimer(this);
    scrubTimer = new QTimer(this);
    spectrumAnalyzerTimer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayerWindow::updateTime);
    connect(scrubTimer, &QTimer::timeout, this, &PlayerWindow::updateTimeScrubber);
    connect(spectrumAnalyzerTimer, &QTimer::timeout, this, &PlayerWindow::updateSpectrumAnalyzer);
    timer->start(timerTimeoutValue);
}

void PlayerWindow::initSpectrumAnalyzer()
{
    SpectrumAnalyzerParameters spectrumAnalyzerParameters;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumData = new double[spectrumAnalyzerParameters.barAmount];

    std::fill(spectrumData, spectrumData + spectrumAnalyzerParameters.barAmount, 0);

    spectrumAnalyzerParameters.barDirection = Qt::Orientation::Vertical;
    /*
    parameters.barDirection = ORIENTATION::HORIZONTAL;
    parameters.barAmount = 2;
    parameters.dimmingPercentage = 30;
    parameters.transparencyPercentage = 55;
    */
    spectrumAnalyzerParameters.peakValue = parameters->spectrumAnalyzerMaximumValue;
    spectrumAnalyzerParameters.barWidthRatio = parameters->spectrumAnalyzerBarWidthRatio;
    spectrumAnalyzerParameters.dimmingRatio = parameters->spectrumAnalyzerDimmingRatio*100;
    spectrumAnalyzerParameters.dimmedTransparencyRatio = parameters->spectrumAnalyzerDimmedTransparencyRatio*100;
    spectrumAnalyzerParameters.discreteParameters.ledHeightRatio = parameters->spectrumAnalyzerLedHeightRatio;;
    spectrumAnalyzerParameters.discreteParameters.barLedAmount = this->parameters->spectrumAnalyzerLedAmount;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumAnalyzerParameters.gradientStops = parameters->spectrumAnalyzerGradient;

    ui->spectrumAnalyzer->setParameters(spectrumAnalyzerParameters);
}

void PlayerWindow::initVuMeter()
{
    SpectrumAnalyzerParameters vuMeterParameters;

    vuMeterParameters.barDirection = Qt::Orientation::Vertical;
    vuMeterParameters.barAmount = 1;

    vuMeterParameters.peakValue = parameters->vuMeterMaximumValue;
    vuMeterParameters.floorValue = parameters->vuMeterMinimumValue;
    vuMeterParameters.barWidthRatio = 1;
    vuMeterParameters.dimmingRatio = parameters->vuMeterDimmingRatio*100;
    vuMeterParameters.dimmedTransparencyRatio = parameters->vuMeterDimmedTransparencyRatio*100;
    vuMeterParameters.discreteParameters.ledHeightRatio = parameters->vuMeterLedHeightRatio;;
    vuMeterParameters.discreteParameters.barLedAmount = this->parameters->vuMeterLedAmount;
    vuMeterParameters.gradientStops = parameters->vuMeterGradient;

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

void PlayerWindow::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"Resize"<<event->size();
}

void PlayerWindow::showEvent(QShowEvent *event)
{
    resize(parameters->playerWindowSize);
}

QString PlayerWindow::getSupportedExtensionsAsString() {
    std::vector<std::string> supportedExtensions = modulePlayer.getSupportedExtensions();
    QString supportedExtensionListString;
    for(std::string &supportedExtension : supportedExtensions) {
        supportedExtensionListString += QString::fromStdString("*." + supportedExtension) + " ";
    }
    supportedExtensionListString +=  "mod.*  nst.*";
    return supportedExtensionListString;
}

QString PlayerWindow::getLessKnownSupportedExtensionsAsString()
{
    std::vector<std::string> lessKnownExtensions = modulePlayer.getSupportedExtensions();

    eraseElementFromVector<std::string>(lessKnownExtensions, "mod");
    eraseElementFromVector<std::string>(lessKnownExtensions, "nst");
    eraseElementFromVector<std::string>(lessKnownExtensions, "s3m");
    eraseElementFromVector<std::string>(lessKnownExtensions, "stm");
    eraseElementFromVector<std::string>(lessKnownExtensions, "xm");
    eraseElementFromVector<std::string>(lessKnownExtensions, "it");

    QString lessKnownExtensionListString;
    for(std::string &lessKnownExtension : lessKnownExtensions) {
        lessKnownExtensionListString += QString::fromStdString("*." + lessKnownExtension) + " ";
    }

    return lessKnownExtensionListString.trimmed();
}

void PlayerWindow::onVolumeChangeRequested(int value) {
    double linearVolume = ((double)value)/100.0f;
    double exponentialVolume = DSP::DSP<double>::calculateExponetialVolume(linearVolume);
    modulePlayer.setVolume(exponentialVolume);
    //qDebug()<<"Linear Volume: "<<linearVolume;
    //qDebug()<<"Exponential Volume "<<exponentialVolume;
}

void PlayerWindow::onTimeScrubbingRequested(int position)
{

}

void PlayerWindow::onFileLoaded() {
    std::string songTitle = modulePlayer.getSongTitle();
    QString title = QString::fromUtf8(songTitle);
    if(title.trimmed().isEmpty())
        title = QString::fromStdString(modulePlayer.getFilePath().stem().string());
    ui->lcdPanel->setSongTitle(title);

    QFontMetrics fontMetrics = ui->titleBar->getFontMetrics();

    QString fileName = QString::fromStdWString(modulePlayer.getFilePath().filename().wstring());
    QString windowTitle = QString("ModPlug Player - ") + fileName;
    int maxLen = 320;

    auto boundingRect = fontMetrics.boundingRect(windowTitle);
    int width=boundingRect.width();
    if(width > maxLen) {
        while(width > maxLen && !windowTitle.isEmpty()) {
            windowTitle = windowTitle.mid(0, windowTitle.length() - 1);
            boundingRect = fontMetrics.boundingRect(windowTitle + "...");
            width = boundingRect.width();
        }
        windowTitle += "...";
    }

    ui->titleBar->setTitle(windowTitle);
    size_t duration = modulePlayer.getSongDuration();
    ui->lcdPanel->setSongDuration(duration);
	ui->timeScrubber->setEnabled(true);
}

void PlayerWindow::onFileOpeningRequested(){
    modulePlayer.stop();
    QString filePath;

    filePath = fileDialog->getOpenFileName(this, "Open Module File",
                                           QString(), tr("All Modules") + " (" + getSupportedExtensionsAsString() + ")"
                                               + " ;; " + tr("Module Lists") + " (*.mol)"
                                               + " ;; " + tr("Compressed Modules") + " (*.mdz *.s3z *.xmz *.itz)"
                                               + " ;; " + tr("ProTracker Modules") + " (*.mod *.nst mod.* nst.*)"
                                               + " ;; " + tr("ScreamTracker Modules") + " (*.s3m *.stm)"
                                               + " ;; " + tr("FastTracker Modules") + " (*.xm)"
                                               + " ;; " + tr("ImpulseTracker Modules") + " (*.it)"
                                               + " ;; " + tr("Other Modules") + " (" + getLessKnownSupportedExtensionsAsString() + ")"
                                               + " ;; " + tr("Wave Files") + " (*.wav)"
                                               + " ;; " + tr("All Files") + " (*.*)"
                                           );
    if (!filePath.isEmpty()){
        emit(loaded(filePath.toStdWString(), true));
    }
}

void PlayerWindow::onAboutWindowRequested() {
    AboutWindow aboutWindow(this);
//    aboutWindow.setModal(true);
    aboutWindow.exec();
}

void PlayerWindow::onPlayListEditorWindowRequested(bool turnOn) {
    if(turnOn) {
        playListEditorWindow->show();
    }
    else {
        playListEditorWindow->hide();
    }
    ui->actionPlayListEditor->setChecked(turnOn);
    ui->optionButtons->togglePlayListEditorButton(turnOn);
}

void PlayerWindow::onRepeatModeToggleRequested()
{
    ModPlugPlayer::RepeatMode currentRepeatMode = parameters->repeatMode;
    emit repeatModeChangeRequested(currentRepeatMode++);
}

void PlayerWindow::onPlayListEditorIsHidden()
{
    ui->actionPlayListEditor->setChecked(false);
    ui->optionButtons->togglePlayListEditorButton(false);
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
    if(parameters->hideByCloseButton) {
        qDebug()<<"Close requested but hide";
        hide();
    }
    else {
        QApplication::exit();
    }
}

void PlayerWindow::onTitleBarHidingStateChangeRequested(bool hide) {
	if(hide) {
		ui->titleBar->hide();
	}
	else {
		ui->titleBar->show();
    }
    ui->actionHideTitleBar->setChecked(hide);
    parameters->hideTitleBar = hide;
}

bool PlayerWindow::isTitleBarHidden() const
{
    return ui->titleBar->isHidden();
}

void PlayerWindow::setSpectrumAnalyzerType(BarType barType)
{
    ui->spectrumAnalyzer->setBarType(barType);
}

void PlayerWindow::setVuMeterType(BarType barType)
{
    ui->vuMeter->setBarType(barType);
}

void PlayerWindow::setSpectrumAnalyzerMaximumValue(int maximumValue)
{
    ui->spectrumAnalyzer->setPeakValue(maximumValue);
    spectrumAnalyzerAnimator->setMaxValue(maximumValue);
}

void PlayerWindow::setSpectrumAnalyzerLedAmount(int ledAmount)
{
    ui->spectrumAnalyzer->setLedAmount(ledAmount);
}

void PlayerWindow::setSpectrumAnalyzerLedHeightRatio(double ledRatio)
{
    ui->spectrumAnalyzer->setLedHeightRatio(ledRatio);
}

void PlayerWindow::setSpectrumAnalyzerBarWidthRatio(double barRatio)
{
    ui->spectrumAnalyzer->setBarWidthRatio(barRatio);
}

void PlayerWindow::setSpectrumAnalyzerDimmingRatio(double dimmingRatio)
{
    ui->spectrumAnalyzer->setDimmingRatio(dimmingRatio);
}

void PlayerWindow::setSpectrumAnalyzerDimmedTransparencyRatio(double dimmedTransparencyRatio)
{
    ui->spectrumAnalyzer->setDimmedTransparencyRatio(dimmedTransparencyRatio);
}

void PlayerWindow::setSpectrumAnalyzerBarAmount(int barAmount)
{
    ui->spectrumAnalyzer->setBarAmount(barAmount);
}

void PlayerWindow::setSpectrumAnalyzerGradient(const QGradientStops & gradient)
{
    ui->spectrumAnalyzer->setGradient(gradient);
}

void PlayerWindow::setVuMeterMaximumValue(int maximumValue)
{
    ui->vuMeter->setPeakValue(maximumValue);
    vuMeterAnimator->setMaxValue(maximumValue);
}

void PlayerWindow::setVuMeterMinimumValue(int minimumValue)
{
    ui->vuMeter->setFloorValue(minimumValue);
    vuMeterAnimator->setMinValue(minimumValue);
}

void PlayerWindow::setVuMeterLedAmount(int ledAmount)
{
    ui->vuMeter->setLedAmount(ledAmount);
}

void PlayerWindow::setVuMeterLedHeightRatio(double ledRatio)
{
    ui->vuMeter->setLedHeightRatio(ledRatio);
}

void PlayerWindow::setVuMeterDimmingRatio(double dimmingRatio)
{
    ui->vuMeter->setDimmingRatio(dimmingRatio);
}

void PlayerWindow::setVuMeterDimmedTransparencyRatio(double dimmedTransparencyRatio)
{
    ui->vuMeter->setDimmedTransparencyRatio(dimmedTransparencyRatio);
}

void PlayerWindow::setVuMeterGradient(const QGradientStops & gradient)
{
    ui->vuMeter->setGradient(gradient);
}

void PlayerWindow::setSpectrumAnalyzerWindowFunction(WindowFunction windowFunction)
{
    parameters->spectrumAnalyzerWindowFunction = windowFunction;
    modulePlayer.setSpectrumAnalyzerWindowFunction(windowFunction);
}

void PlayerWindow::onKeepStayingViewPortRequested(bool keepStayingInViewPort)
{
    moveByMouseClick->setKeepStayingInViewPort(keepStayingInViewPort);
    parameters->keepStayingInViewPort = keepStayingInViewPort;
}

void PlayerWindow::onChangeSnapThresholdRequested(int snappingThreshold)
{
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
    parameters->snappingThreshold = snappingThreshold;
}

void PlayerWindow::selectNewSoundOutput(PaDeviceIndex deviceIndex)
{
    modulePlayer.pause();
    modulePlayer.setOutputDeviceIndex(deviceIndex);
    modulePlayer.play();
}

void PlayerWindow::onLoaded(std::filesystem::path filePath, bool successfull) {
    playingMode = PlayingMode::SingleTrack;
}

//TODO: This is not needed, remove it. Playlist item should be opened automatically when it is played
void PlayerWindow::onLoaded(PlayListItem playListItem, bool successfull) {
    playingMode = PlayingMode::PlayList;
    qDebug()<<"Play "<<playListItem.filePath;
}

void PlayerWindow::onStopRequested()
{
    //    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->stop();
    qDebug()<<"Stop";
}

void PlayerWindow::onStopRequested(PlayListItem playListItem)
{

}

void PlayerWindow::onPlayRequested()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    spectrumAnalyzerTimer->start(spectrumAnalyzerTimerTimeoutValue);
    qDebug()<<"Play";
}

void PlayerWindow::onPlayRequested(PlayListItem playListItem)
{
    emit(playingStarted(playListItem));
    onPlayRequested();
    qDebug()<< "onPlayingStarted" << playListItem.title;
}

void PlayerWindow::onPauseRequested()
{
//    if(playerState != PLAYERSTATE::STOPPED)
    qDebug()<<"Pause";
}

void PlayerWindow::onPauseRequested(PlayListItem playListItem)
{

}

void PlayerWindow::onResumeRequested()
{

}

void PlayerWindow::onResumeRequested(PlayListItem playListItem)
{

}

void PlayerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void PlayerWindow::dropEvent(QDropEvent *event)
{
    modulePlayer.stop();
    emit openRequested(event->mimeData()->urls()[0].toLocalFile().toStdWString());
    emit ui->playerControlButtons->play();
    event->setDropAction(Qt::LinkAction);
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
    parameters->playerWindowSize = size();
    if(parameters->hideByCloseButton) {
        hide();
        event->ignore();
    }
    else {
        parameters->playerWindowSize.save(settings);

        event->accept();
    }
    //
}

void PlayerWindow::onAlwaysOnTopStateChangeRequested(bool alwaysOnTop) {
    WindowUtil::setAlwaysOnTop(this, alwaysOnTop);
    ui->actionAlways_On_Top->setChecked(alwaysOnTop);
    parameters->alwaysOnTop = alwaysOnTop;
}

void PlayerWindow::onSnappingToViewPortStateChangeRequested(bool snapToViewPort) {
    ui->actionSnap_to_Viewport->setChecked(snapToViewPort);
    moveByMouseClick->setSnapToViewPort(snapToViewPort);
    parameters->snapToViewPort = snapToViewPort;
}

void PlayerWindow::onSnappingThresholdChangeRequested(int snappingThreshold)
{
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
}

void PlayerWindow::onKeepingStayingInViewPortStateChangeRequested(bool keepStayingInViewPort) {
    ui->actionKeep_Staying_in_ViewPort->setChecked(keepStayingInViewPort);
    moveByMouseClick->setKeepStayingInViewPort(keepStayingInViewPort);
    parameters->keepStayingInViewPort = keepStayingInViewPort;
}

void PlayerWindow::onPreviousRequested() {

}

void PlayerWindow::onNextRequested() {

}

void PlayerWindow::onRepeatModeChangeRequested(ModPlugPlayer::RepeatMode repeatMode) {
    parameters->repeatMode = repeatMode;
    ui->lcdPanel->setRepeatMode(repeatMode);
}

void PlayerWindow::onEqStateChangeRequested(const bool activated)
{

}

void PlayerWindow::onAGCStateChangeRequested(const bool activated)
{

}

void PlayerWindow::onXBassStateChangeRequested(const bool activated)
{

}

void PlayerWindow::onSurroundStateChangeRequested(const bool activated)
{

}

void PlayerWindow::onReverbStateChangeRequested(const bool activated)
{

}

void PlayerWindow::onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter)
{
    qDebug()<<"Interpolation filter";
    modulePlayer.setInterpolationFilter(interpolationFilter);
    emit interpolationFilterChanged(interpolationFilter);
}
