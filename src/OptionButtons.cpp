#include "OptionButtons.hpp"
#include "ui_OptionButtons.h"

OptionButtons::OptionButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionButtons)
{
    ui->setupUi(this);
    connect(ui->infoButton, SIGNAL(clicked()), this, SIGNAL(info()));
    connect(ui->repeatButton, SIGNAL(clicked()), this, SIGNAL(repeat()));
    connect(ui->saveAsButton, SIGNAL(clicked()), this, SIGNAL(saveAs()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SIGNAL(del()));
    connect(ui->detailsButton, SIGNAL(clicked()), this, SIGNAL(details()));
    connect(ui->shuffleButton, SIGNAL(clicked()), this, SIGNAL(shuffle()));
    connect(ui->saveListButton, SIGNAL(clicked()), this, SIGNAL(saveList()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SIGNAL(about()));
    connect(ui->playlistButton, SIGNAL(clicked()), this, SIGNAL(playlist()));
    connect(ui->saveWavButton, SIGNAL(clicked()), this, SIGNAL(saveWav()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SIGNAL(remove()));
}

void OptionButtons::setActiveButtonLightColor(const RGB & color)
{
    ui->infoButton->setActiveButtonLightColor(color);
    ui->repeatButton->setActiveButtonLightColor(color);
    ui->saveAsButton->setActiveButtonLightColor(color);
    ui->deleteButton->setActiveButtonLightColor(color);
    ui->detailsButton->setActiveButtonLightColor(color);
    ui->shuffleButton->setActiveButtonLightColor(color);
    ui->saveListButton->setActiveButtonLightColor(color);
    ui->clearButton->setActiveButtonLightColor(color);
    ui->aboutButton->setActiveButtonLightColor(color);
    ui->playlistButton->setActiveButtonLightColor(color);
    ui->saveWavButton->setActiveButtonLightColor(color);
    ui->removeButton->setActiveButtonLightColor(color);
}

void OptionButtons::setInactiveButtonLightColor(const RGB & color)
{
    ui->infoButton->setInactiveButtonLightColor(color);
    ui->repeatButton->setInactiveButtonLightColor(color);
    ui->saveAsButton->setInactiveButtonLightColor(color);
    ui->deleteButton->setInactiveButtonLightColor(color);
    ui->detailsButton->setInactiveButtonLightColor(color);
    ui->shuffleButton->setInactiveButtonLightColor(color);
    ui->saveListButton->setInactiveButtonLightColor(color);
    ui->clearButton->setInactiveButtonLightColor(color);
    ui->aboutButton->setInactiveButtonLightColor(color);
    ui->playlistButton->setInactiveButtonLightColor(color);
    ui->saveWavButton->setInactiveButtonLightColor(color);
    ui->removeButton->setInactiveButtonLightColor(color);
}

OptionButtons::~OptionButtons()
{
    delete ui;
}
