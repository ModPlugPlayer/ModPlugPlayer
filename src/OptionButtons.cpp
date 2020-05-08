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

OptionButtons::~OptionButtons()
{
    delete ui;
}
