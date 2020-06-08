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

	QString iconSvgData = getResourceContent(":/Graphics/Vectoral/play.svg");
	/*
	QString iconSvgData = " <svg width=\"400\" height=\"110\">\
			<rect width=\"300\" height=\"100\" style=\"fill:${color};stroke-width:3;stroke:rgb(0,0,0)\" />\
		  </svg> ";*/
	iconSvgData = iconSvgData.replace("#00ff00", "#ffff00");
	QIcon theIcon(new SVGIconEngine(iconSvgData));
		ui->playButton->setIcon(theIcon);
		//ui->playButton->setIconSize(QSize(20,7));
		//ui->playButton->setText("");

}

PlayerControlButtons::~PlayerControlButtons()
{
	delete ui;
}

QString PlayerControlButtons::
getResourceContent(std::string resource)
{
	QString data;

	QFile file(resource.c_str());
	if(!file.open(QIODevice::ReadOnly)) {
		qDebug()<<"filenot opened"<<endl;
	}
	else
	{
		qDebug()<<"file opened"<<endl;
		data = file.readAll();
	}

	file.close();
	return data;
}
