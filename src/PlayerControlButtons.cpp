#include "PlayerControlButtons.hpp"
#include "ui_PlayerControlButtons.h"
#include <QDebug>
#include "SVGIconEngine.hpp"
#include "ResourceUtil.hpp"

PlayerControlButtons::PlayerControlButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControlButtons)
{
    ui->setupUi(this);

	state = PlayerState::Stopped;
    connect(ui->openButton,     SIGNAL(clicked()), this, SIGNAL(open()));
    connect(ui->playButton,     SIGNAL(clicked()), this, SIGNAL(play()));
    connect(ui->pauseButton,    SIGNAL(clicked()), this, SIGNAL(pause()));
    connect(ui->stopButton,     SIGNAL(clicked()), this, SIGNAL(stop()));
    connect(ui->setupButton,    SIGNAL(clicked()), this, SIGNAL(setup()));
    connect(ui->rewindButton,   SIGNAL(clicked()), this, SIGNAL(rewind()));
    connect(ui->fastForwardButton,  SIGNAL(clicked()), this, SIGNAL(fastForward()));
    connect(ui->previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));
    connect(ui->nextButton,     SIGNAL(clicked()), this, SIGNAL(next()));

    iconOpen = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/eject.svg"), "#00ff00");
    iconPlay = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/play.svg"), "#00ff00");
    iconPause = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/pause.svg"), "#00ff00");
    iconStop = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/stop.svg"), "#00ff00");
    iconRewind = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/rewind.svg"), "#00ff00");
    iconFastForward = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/fastforward.svg"), "#00ff00");
    iconPrevious = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/previous.svg"), "#00ff00");
    iconNext = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/next.svg"), "#00ff00");

	/*
	QString iconSvgData = " <svg width=\"400\" height=\"110\">\
			<rect width=\"300\" height=\"100\" style=\"fill:${color};stroke-width:3;stroke:rgb(0,0,0)\" />\
		  </svg> ";*/
	/*
	QString iconSvgData = iconSvgDataPlay.replace("#00ff00", "#ffff00");
	QIcon theIcon(new SVGIconEngine(iconSvgData));
		ui->playButton->setIcon(theIcon);
	*/
		//ui->playButton->setIconSize(QSize(20,7));
		//ui->playButton->setText("");
}

PlayerControlButtons::~PlayerControlButtons() {
	iconOpen->deleteLater();
	iconPlay->deleteLater();
	iconPause->deleteLater();
	iconStop->deleteLater();
	iconRewind->deleteLater();
	iconFastForward->deleteLater();
	iconPrevious->deleteLater();
	iconNext->deleteLater();
	delete ui;
}

void PlayerControlButtons::setActiveButtonLightColor(const RGB &color) {
    LedButton::setActiveButtonLightColor(color);
	iconOpen->setActiveColor(color);
	iconPlay->setActiveColor(color);
	iconPause->setActiveColor(color);
	iconStop->setActiveColor(color);
	iconRewind->setActiveColor(color);
	iconFastForward->setActiveColor(color);
	iconPrevious->setActiveColor(color);
	iconNext->setActiveColor(color);

	refresh();
}

void PlayerControlButtons::setInactiveButtonLightColor(const RGB &color) {
    LedButton::setInactiveButtonLightColor(color);
	iconOpen->setInactiveColor(color);
	iconPlay->setInactiveColor(color);
	iconPause->setInactiveColor(color);
	iconStop->setInactiveColor(color);
	iconRewind->setInactiveColor(color);
	iconFastForward->setInactiveColor(color);
	iconPrevious->setInactiveColor(color);
	iconNext->setInactiveColor(color);

    refresh();
}

void PlayerControlButtons::setBackgroundColor(const RGB & color) {
    PlayerButton::setBackgroundColor(color);
    QString style = QString("#PlayerWindow{background-color:%1;}; #PlayerControlButtons{color:%2;}").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    this->setStyleSheet(style);
}

void PlayerControlButtons::setTextColor(const RGB & color) {
    PlayerButton::setTextColor(color);
    QString style = QString("#PlayerWindow{background-color:%1;}; #PlayerControlButtons{color:%2;}").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    this->setStyleSheet(style);
}

void PlayerControlButtons::refresh() {
	ui->playButton->setIcon(state == PlayerState::Playing ? iconPlay->getActiveIcon() : iconPlay->getInactiveIcon());
	ui->pauseButton->setIcon(state == PlayerState::Paused ? iconPause->getActiveIcon() : iconPause->getInactiveIcon());
	ui->stopButton->setIcon(state == PlayerState::Stopped ? iconStop->getActiveIcon() : iconStop->getInactiveIcon());
	ui->rewindButton->setIcon(iconRewind->getInactiveIcon());
	ui->fastForwardButton->setIcon(iconFastForward->getInactiveIcon());
	ui->nextButton->setIcon(iconNext->getInactiveIcon());
	ui->previousButton->setIcon(iconPrevious->getInactiveIcon());
	ui->openButton->setIcon(iconOpen->getInactiveIcon());
}

void PlayerControlButtons::setState(const PlayerState &state) {
	this->state = state;
	refresh();
}

void PlayerControlButtons::on_playerState_changed(PlayerState playerState) {
	this->state = playerState;
	this->refresh();
}

void PlayerControlButtons::on_openButton_pressed() {
	ui->openButton->setIcon(iconOpen->getActiveIcon());
}

void PlayerControlButtons::on_openButton_released() {
	ui->openButton->setIcon(iconOpen->getInactiveIcon());
}

void PlayerControlButtons::on_rewindButton_pressed() {
	ui->rewindButton->setIcon(iconRewind->getActiveIcon());
}

void PlayerControlButtons::on_rewindButton_released() {
	ui->rewindButton->setIcon(iconRewind->getInactiveIcon());
}

void PlayerControlButtons::on_fastForwardButton_pressed() {
	ui->fastForwardButton->setIcon(iconFastForward->getActiveIcon());
}

void PlayerControlButtons::on_fastForwardButton_released() {
	ui->fastForwardButton->setIcon(iconFastForward->getInactiveIcon());
}

void PlayerControlButtons::on_previousButton_pressed() {
	ui->previousButton->setIcon(iconPrevious->getActiveIcon());
}

void PlayerControlButtons::on_previousButton_released() {
	ui->previousButton->setIcon(iconPrevious->getInactiveIcon());
}

void PlayerControlButtons::on_nextButton_pressed() {
	ui->nextButton->setIcon(iconNext->getActiveIcon());
}

void PlayerControlButtons::on_nextButton_released() {
	ui->nextButton->setIcon(iconNext->getInactiveIcon());
}

void PlayerControlButtons::on_playButton_pressed() {
	ui->playButton->setIcon(iconPlay->getActiveIcon());
}

void PlayerControlButtons::on_playButton_released() {
	if(state != PlayerState::Playing)
		ui->playButton->setIcon(iconPlay->getInactiveIcon());
}

void PlayerControlButtons::on_pauseButton_pressed() {
	ui->pauseButton->setIcon(iconPause->getActiveIcon());
}

void PlayerControlButtons::on_pauseButton_released() {
	if(state != PlayerState::Paused)
		ui->pauseButton->setIcon(iconPause->getInactiveIcon());
}

void PlayerControlButtons::on_stopButton_pressed() {
	ui->stopButton->setIcon(iconStop->getActiveIcon());
}

void PlayerControlButtons::on_stopButton_released() {
    if(state != PlayerState::Stopped)
        ui->stopButton->setIcon(iconStop->getInactiveIcon());
}

void PlayerControlButtons::refreshStyleSheet()
{

}
