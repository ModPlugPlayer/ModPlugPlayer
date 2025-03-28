/*
PlayerControlButtons class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerControlButtons.hpp"
#include "ui_PlayerControlButtons.h"
#include <QDebug>
#include <QOverload>
#include <MessageCenter.hpp>
#include <Util/ResourceUtil.hpp>

PlayerControlButtons::PlayerControlButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControlButtons)
{
    ui->setupUi(this);

    buttons.push_back(ui->openButton);
    buttons.push_back(ui->playButton);
    buttons.push_back(ui->pauseButton);
    buttons.push_back(ui->stopButton);
    buttons.push_back(ui->setupButton);
    buttons.push_back(ui->rewindButton);
    buttons.push_back(ui->fastForwardButton);
    buttons.push_back(ui->previousButton);
    buttons.push_back(ui->nextButton);

	state = PlayerState::Stopped;
    connect(ui->openButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::openRequested));
    connect(ui->playButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::playRequested));
    connect(ui->pauseButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::pauseRequested));
    connect(ui->stopButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::stopRequested));
    connect(ui->setupButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.windowStandardRequests.settingsWindowRequests, &MessageCenterRequests::WindowStandardRequests::windowOpenRequested);
    connect(ui->rewindButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::rewindRequested));
    connect(ui->fastForwardButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::fastForwardRequested));
    connect(ui->previousButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::previousRequested));
    connect(ui->nextButton, &SVGLEDButton::clicked, &MessageCenter::getInstance().requests.songRequests, qOverload<>(&MessageCenterRequests::SongRequests::nextRequested));

    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::playingStarted), this, &PlayerControlButtons::onPlayingStarted);
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::paused), this, &PlayerControlButtons::onPaused);
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::resumed), this, &PlayerControlButtons::onResumed);
    connect(&MessageCenter::getInstance().events.songEvents, qOverload<>(&MessageCenterEvents::SongEvents::stopped), this, &PlayerControlButtons::onStopped);

    iconOpen = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/eject.svg"), "#00ff00");
    iconPlay = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/play.svg"), "#00ff00");
    iconPause = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/pause.svg"), "#00ff00");
    iconStop = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/stop.svg"), "#00ff00");
    iconRewind = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/rewind.svg"), "#00ff00");
    iconFastForward = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/fastforward.svg"), "#00ff00");
    iconPrevious = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/previous.svg"), "#00ff00");
    iconNext = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/next.svg"), "#00ff00");

    icons.push_back(iconOpen);
    icons.push_back(iconPlay);
    icons.push_back(iconPause);
    icons.push_back(iconStop);
    icons.push_back(iconRewind);
    icons.push_back(iconFastForward);
    icons.push_back(iconPrevious);
    icons.push_back(iconNext);
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
    LEDColorProperties::setActiveLEDColor(color);
    for(SVGIcon * icon : icons) {
        icon->setActiveColor(color);
    }
	refresh();
}

void PlayerControlButtons::setInactiveButtonLightColor(const RGB &color) {
    LEDColorProperties::setInactiveLEDColor(color);
    for(SVGIcon * icon : icons) {
        icon->setInactiveColor(color);
    }

    refresh();
}

void PlayerControlButtons::setBackgroundColor(const RGB & color) {
    TextColorProperties::setBackgroundColor(color);
    refreshStyleSheet();
}

void PlayerControlButtons::setTextColor(const RGB & color) {
    TextColorProperties::setTextColor(color);
    refreshStyleSheet();
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

void PlayerControlButtons::onPlayerStateChanged(PlayerState playerState) {
	this->state = playerState;
    // \todo: The two lines below crashes the application when loading a music. Investigate the problem.
    //ui->playButton->setLocked(state == PlayerState::Playing);
    //ui->stopButton->setLocked(state == PlayerState::Stopped);
    this->refresh();
}

void PlayerControlButtons::onPlayingStarted() {
    this->state = PlayerState::Playing;
    refresh();
}

void PlayerControlButtons::onPaused() {
    this->state = PlayerState::Paused;
    refresh();
}

void PlayerControlButtons::onResumed() {
    this->state = PlayerState::Playing;
    refresh();
}

void PlayerControlButtons::onStopped() {
    this->state = PlayerState::Stopped;
    refresh();
}

void PlayerControlButtons::refreshStyleSheet()
{
    QString style = QString("background-color:%1; color:%2;").arg(backgroundColor.hex().c_str(), textColor.hex().c_str());
    for(QPushButton * button : buttons) {
        button->setStyleSheet(style);
    }
}
