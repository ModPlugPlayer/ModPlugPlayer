#include "PlayerControlButtons.hpp"
#include "ui_PlayerControlButtons.h"
#include <QDebug>
#include "SVGIconEngine.hpp"

PlayerControlButtons::PlayerControlButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControlButtons)
{
    ui->setupUi(this);

    connect(ui->openButton,     SIGNAL(clicked()), this, SIGNAL(open()));
    connect(ui->playButton,     SIGNAL(clicked()), this, SIGNAL(play()));
    connect(ui->pauseButton,    SIGNAL(clicked()), this, SIGNAL(pause()));
    connect(ui->stopButton,     SIGNAL(clicked()), this, SIGNAL(stop()));
    connect(ui->setupButton,    SIGNAL(clicked()), this, SIGNAL(setup()));
    connect(ui->rewindButton,   SIGNAL(clicked()), this, SIGNAL(rewind()));
    connect(ui->fastForwardButton,  SIGNAL(clicked()), this, SIGNAL(fastForward()));
    connect(ui->previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));
    connect(ui->nextButton,     SIGNAL(clicked()), this, SIGNAL(next()));

	iconOpen = SVGIcon(getResourceContent(":/Graphics/Vectoral/eject.svg"), "#00ff00");
	iconPlay = SVGIcon(getResourceContent(":/Graphics/Vectoral/play.svg"), "#00ff00");
	iconPause = SVGIcon(getResourceContent(":/Graphics/Vectoral/pause.svg"), "#00ff00");
	iconStop = SVGIcon(getResourceContent(":/Graphics/Vectoral/stop.svg"), "#00ff00");
	iconRewind = SVGIcon(getResourceContent(":/Graphics/Vectoral/rewind.svg"), "#00ff00");
	iconFastForward = SVGIcon(getResourceContent(":/Graphics/Vectoral/fastforward.svg"), "#00ff00");
	iconPrevious = SVGIcon(getResourceContent(":/Graphics/Vectoral/previous.svg"), "#00ff00");
	iconNext = SVGIcon(getResourceContent(":/Graphics/Vectoral/next.svg"), "#00ff00");

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

PlayerControlButtons::~PlayerControlButtons()
{
	delete ui;
}

void PlayerControlButtons::setActiveButtonLightColor(const RGB &color){
	activeButtonLightColor = color;

	iconOpen.setActiveColor(color);
	iconPlay.setActiveColor(color);
	iconPause.setActiveColor(color);
	iconStop.setActiveColor(color);
	iconRewind.setActiveColor(color);
	iconFastForward.setActiveColor(color);
	iconPrevious.setActiveColor(color);
	iconNext.setActiveColor(color);

	refresh();
}

void PlayerControlButtons::setInactiveButtonLightColor(const RGB &color){
	inactiveButtonLightColor = color;

	iconOpen.setInactiveColor(color);
	iconPlay.setInactiveColor(color);
	iconPause.setInactiveColor(color);
	iconStop.setInactiveColor(color);
	iconRewind.setInactiveColor(color);
	iconFastForward.setInactiveColor(color);
	iconPrevious.setInactiveColor(color);
	iconNext.setInactiveColor(color);

	refresh();
}

void PlayerControlButtons::refresh() {
	ui->playButton->setIcon(state == PlayerState::Playing ? iconPlay.getActiveIcon() : iconPlay.getInactiveIcon());
	ui->pauseButton->setIcon(state == PlayerState::Paused ? iconPause.getActiveIcon() : iconPause.getInactiveIcon());
	ui->stopButton->setIcon(state == PlayerState::Stopped ? iconStop.getActiveIcon() : iconStop.getInactiveIcon());
	ui->rewindButton->setIcon(iconRewind.getInactiveIcon());
	ui->fastForwardButton->setIcon(iconFastForward.getInactiveIcon());
	ui->nextButton->setIcon(iconNext.getInactiveIcon());
	ui->previousButton->setIcon(iconPrevious.getInactiveIcon());
	ui->openButton->setIcon(iconOpen.getInactiveIcon());
}

void PlayerControlButtons::setState(const PlayerState &state){
	this->state = state;
	refresh();
}

QString PlayerControlButtons::
getResourceContent(std::string resource)
{
	QString data;

	QFile file(resource.c_str());
	if(!file.open(QIODevice::ReadOnly)) {
		qDebug()<<"file not opened"<<endl;
	}
	else
	{
		qDebug()<<"file opened"<<endl;
		data = file.readAll();
	}

	file.close();
	return data;
}

void PlayerControlButtons::on_openButton_pressed()
{
	ui->openButton->setIcon(iconOpen.getActiveIcon());
}

void PlayerControlButtons::on_openButton_released()
{
	ui->openButton->setIcon(iconOpen.getInactiveIcon());
}
