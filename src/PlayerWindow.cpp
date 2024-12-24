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
#include "ModuleHandler.hpp"
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QtGlobal>
#include <SpectrumAnalyzer.hpp>
#include <QMimeData>
#include <DSP.hpp>
#include "AboutWindow.hpp"
#include <QCloseEvent>
#include "SetupWindow.hpp"
#include "Util/WindowUtil.hpp"
#include <QOverload>
#include <VolumeControl.hpp>

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

    qInfo()<<"Settings file location:"<<settings->fileName();

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
    setFixedSize(413,194);
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
    TimeInfo timeInfo = moduleHandler.getTimeInfo();
    ui->timeScrubber->setValue(timeInfo.globalRowIndex);
    emit elapsedTimeChanged(moduleHandler.getTimeInfo().seconds);
    updateSpectrumAnalyzer();
    updateInstantModuleInfo();
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
    if(moduleHandler.isSongState(SongState::Loaded)) {
		if(scrubberClickedPosition != scrubberPreviousValue)
            moduleHandler.scrubTime(scrubberClickedPosition);
		scrubberPreviousValue = scrubberClickedPosition;
	}
}

void PlayerWindow::on_timeScrubber_sliderPressed()
{
    if(moduleHandler.isSongState(SongState::Loaded)) {
		timer->stop();
		scrubberClickedPosition = ui->timeScrubber->value();
		scrubTimer->start(scrubTimerTimeoutValue);
        moduleHandler.scrubTime(scrubberClickedPosition);
	}
}
void PlayerWindow::on_timeScrubber_sliderReleased()
{
    if(moduleHandler.isSongState(SongState::Loaded)) {
	//    updateTime();
		scrubTimer->stop();
        timer->start(timerTimeoutValue);
    }
}

void PlayerWindow::onModuleHandlerStopped() {
    emit stopRequested();
    qDebug()<<"Stop requested";
}

void PlayerWindow::updateSpectrumAnalyzer()
{
    moduleHandler.getSpectrumData(spectrumData);
    if(spectrumAlayzerScaleIsLogarithmic) {
        DSP::DSP<double>::magnitudeToDecibel(spectrumData, spectrumData, spectrumAnalyzerBarAmount);
    }
    spectrumAnalyzerAnimator->setValues(spectrumData);
    spectrumAnalyzerAnimator->getValues(spectrumData);
    float volumeCoefficient = double(ui->volumeControl->value())/100;
    double vuMeterDbValue = moduleHandler.getVuMeterValue();
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
        double barValue = spectrumData[i];
        if(barValue == NAN)
            barValue = 0;
        else if(barValue < 0)
            barValue = 0;
        else if(barValue > parameters->spectrumAnalyzerMaximumValue)
            barValue = parameters->spectrumAnalyzerMaximumValue;
        ui->spectrumAnalyzer->setBarValue(i, barValue);
    }

    ui->spectrumAnalyzer->update();
    ui->vuMeter->update();
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

void PlayerWindow::initVuMeter() {
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

void PlayerWindow::initMenus() {
    ui->menubar->hide();

    ui->actionAbout_ModPlug_Player->setMenuRole(QAction::ApplicationSpecificRole);
    QAction * aboutSeparator = ui->menuFile->addSeparator();
    aboutSeparator->setMenuRole(QAction::ApplicationSpecificRole);

    ui->actionPreferences->setMenuRole(QAction::ApplicationSpecificRole);
}

void PlayerWindow::resizeEvent(QResizeEvent *event) {
    qDebug()<<"Resize"<<event->size();
    updateWindowTitle();
}

void PlayerWindow::showEvent(QShowEvent *event) {
    resize(parameters->playerWindowSize);
}

QString PlayerWindow::getSupportedExtensionsAsString() {
    std::vector<std::string> supportedExtensions = moduleHandler.getSupportedExtensions();
    QString supportedExtensionListString;
    for(std::string &supportedExtension : supportedExtensions) {
        supportedExtensionListString += QString::fromStdString("*." + supportedExtension) + " ";
    }
    supportedExtensionListString +=  "mod.*  nst.*";
    return supportedExtensionListString;
}

QString PlayerWindow::getLessKnownSupportedExtensionsAsString()
{
    std::vector<std::string> lessKnownExtensions = moduleHandler.getSupportedExtensions();

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
    double exponentialVolume = DSP::VolumeControl<double>::calculateExponetialVolume(linearVolume);
    moduleHandler.setVolume(exponentialVolume);
    qDebug()<<"Requested linear Volume is"<<linearVolume;
    qDebug()<<"Volume is set to"<<exponentialVolume<<"as exponantial volume";
}

void PlayerWindow::onTimeScrubbingRequested(const int position) {

}

void PlayerWindow::onTimeScrubbed(const int position) {

}

void PlayerWindow::onLoaded(const std::filesystem::path filePath, const bool successfull) {

}

void PlayerWindow::onLoaded(const ModuleFileInfo fileInfo, const bool successfull) {
    if(!successfull) {
        return; // To-do: warn user that the file can't be loaded
    }
    currentModuleFileInfo = fileInfo;
    playingMode = PlayingMode::SingleTrack;
    std::string songTitle = moduleHandler.getSongTitle();
    QString title = QString::fromUtf8(songTitle);
    if(title.trimmed().isEmpty())
        title = QString::fromStdString(moduleHandler.getFilePath().stem().string());
    emit trackTitleChanged(title);

    updateWindowTitle();

    emit trackDurationChanged(fileInfo.moduleInfo.songDuration);
    emit moduleFormatChanged(QString::fromStdString(fileInfo.moduleInfo.moduleFormat).toUpper());
    emit channelAmountChanged(moduleHandler.getChannelAmount());
    emit activeChannelAmountChanged(moduleHandler.getActiveChannelAmount());
    emit subSongAmountChanged(moduleHandler.getSubSongAmount());
    emit currentSubSongIndexChanged(moduleHandler.getCurrentSubSongIndex());
    emit patternAmountChanged(moduleHandler.getPatternAmount());
    emit currentPatternIndexChanged(moduleHandler.getCurrentPatternIndex());
    ui->timeScrubber->setEnabled(true);
    emit ui->playerControlButtons->play();
}

void PlayerWindow::updateInstantModuleInfo(){
    if(moduleHandler.getPlayerState() == PlayerState::Playing) {
        emit activeChannelAmountChanged(moduleHandler.getActiveChannelAmount());
        emit currentSubSongIndexChanged(moduleHandler.getCurrentSubSongIndex());
        emit patternAmountChanged(moduleHandler.getPatternAmount());
        emit currentPatternIndexChanged(moduleHandler.getCurrentPatternIndex());
    }
}

void PlayerWindow::updateWindowTitle() {
    QString titleBarText = QString("ModPlug Player - ") + currentModuleFileInfo.filePath.filename().c_str();
    QString stem = currentModuleFileInfo.filePath.stem().c_str();
    QString extension = currentModuleFileInfo.filePath.extension().c_str();
    if(extension.size() <= 4)
        titleBarText = WindowUtil::shortenTextToWidth(ui->titleBar->labelFont(), ui->titleBar->labelWidth(), QString("ModPlug Player - ") + stem, extension);
    else
        titleBarText = WindowUtil::shortenTextToWidth(ui->titleBar->labelFont(), ui->titleBar->labelWidth(), titleBarText);
    ui->titleBar->setTitle(titleBarText);
}

void PlayerWindow::onFileOpeningRequested() {
    moduleHandler.stop();
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
        std::filesystem::path path(filePath.toStdString());
        emit(openRequested(path));
    }
}

void PlayerWindow::onAboutWindowRequested() {
    AboutWindow aboutWindow(this);
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

void PlayerWindow::onAmigaFilterToggleRequested() {
    ModPlugPlayer::AmigaFilter currentAmigaFilter = parameters->amigaFilter;
    emit amigaFilterChangeRequested(currentAmigaFilter++);
}

void PlayerWindow::onInterpolationFilterToggleRequested() {
    ModPlugPlayer::InterpolationFilter currentInterpolationFilter = parameters->interpolationFilter;
    emit interpolationFilterChangeRequested(currentInterpolationFilter++);
}

void PlayerWindow::onEqToggleRequested() {
    emit eqStateChangeRequested(!parameters->eqEnabled);
}

void PlayerWindow::onDSPToggleRequested() {
    emit dspStateChangeRequested(!parameters->dspEnabled);
}

void PlayerWindow::onDSPOpToggleRequested() {

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
    spectrumAnalyzerBarAmount = barAmount;
    ui->spectrumAnalyzer->setBarAmount(barAmount);
}

void PlayerWindow::setSpectrumAnalyzerGradient(const QGradientStops & gradient)
{
    ui->spectrumAnalyzer->setGradient(gradient);
}

void PlayerWindow::setSpectrumAnalyzerScaleToLogarithmic(bool isLogarithmicScale) {
    this->spectrumAlayzerScaleIsLogarithmic = isLogarithmicScale;
    parameters->spectrumAnalyzerScaleIsLogarithmic = isLogarithmicScale;
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

void PlayerWindow::setSpectrumAnalyzerWindowFunction(WindowFunction windowFunction) {
    parameters->spectrumAnalyzerWindowFunction = windowFunction;
    moduleHandler.setSpectrumAnalyzerWindowFunction(windowFunction);
}

void PlayerWindow::onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) {
    ui->actionKeep_Staying_in_ViewPort->setChecked(keepStayingInViewPort);
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
    moduleHandler.pause();
    moduleHandler.setOutputDeviceIndex(deviceIndex);
    moduleHandler.play();
}

void PlayerWindow::onOpenRequested(const std::filesystem::path filePath) {

}

void PlayerWindow::onOpenRequested(const PlayListItem playListItem) {

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
    moduleHandler.stop();
    emit timeScrubbed(0);
}

void PlayerWindow::onStopRequested(const PlayListItem playListItem)
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
    moduleHandler.stop();
    emit openRequested(event->mimeData()->urls()[0].toLocalFile().toStdWString());
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
}

void PlayerWindow::onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) {
    WindowUtil::setAlwaysOnTop(this, alwaysOnTop);
    ui->actionAlways_On_Top->setChecked(alwaysOnTop);
    parameters->alwaysOnTop = alwaysOnTop;
}

void PlayerWindow::onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) {
    ui->actionSnap_to_Viewport->setChecked(snapToViewPort);
    moveByMouseClick->setSnapToViewPort(snapToViewPort);
    parameters->snapToViewPort = snapToViewPort;
}

void PlayerWindow::onSnappingThresholdChangeRequested(const int snappingThreshold)
{
    moveByMouseClick->setSnappingThreshold(snappingThreshold);
}

void PlayerWindow::onPreviousRequested() {

}

void PlayerWindow::onNextRequested() {

}

void PlayerWindow::onRepeatModeChangeRequested(const ModPlugPlayer::RepeatMode repeatMode) {
    moduleHandler.setRepeatMode(repeatMode);
    emit repeatModeChanged(repeatMode);
}

void PlayerWindow::onEqStateChangeRequested(const bool activated) {
    parameters->eqEnabled = activated;
    qInfo() << "Equalizer state was set to" << activated;
    emit eqStateChanged(activated);
}

void PlayerWindow::onDSPStateChangeRequested(const bool activated) {
    parameters->dspEnabled = activated;
    qInfo() << "DSP state was set to" << activated;
    emit dspStateChanged(activated);
}

void PlayerWindow::onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) {
    parameters->amigaFilter = amigaFilter;
    moduleHandler.setAmigaFilter(amigaFilter);
    qInfo()<<"Amiga filter changed to" << (int) amigaFilter;
    emit amigaFilterChanged(amigaFilter);
}



void PlayerWindow::onInterpolationFilterChangeRequested(const ModPlugPlayer::InterpolationFilter interpolationFilter)
{
    parameters->interpolationFilter = interpolationFilter;
    moduleHandler.setInterpolationFilter(interpolationFilter);
    qInfo()<<"Interpolation filter changed to" << (int) interpolationFilter;
    emit interpolationFilterChanged(interpolationFilter);
}

void PlayerWindow::onRepeatModeChanged(const RepeatMode repeatMode) {
    parameters->repeatMode = repeatMode;
}
