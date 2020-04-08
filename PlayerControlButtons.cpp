#include "PlayerControlButtons.hpp"
#include "ui_PlayerControlButtons.h"
#include <QDebug>

PlayerControlButtons::PlayerControlButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerControlButtons)
{
    ui->setupUi(this);
    fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setNameFilter(tr("All Modules (*.mod *.xm *.it)"));

//    connect(ui->openButton,     SIGNAL(clicked()), this, SIGNAL(open()));
    connect(ui->playButton,     SIGNAL(clicked()), this, SIGNAL(play()));
    connect(ui->pauseButton,    SIGNAL(clicked()), this, SIGNAL(pause()));
    connect(ui->stopButton,     SIGNAL(clicked()), this, SIGNAL(stop()));
    connect(ui->setupButton,    SIGNAL(clicked()), this, SIGNAL(setup()));
    connect(ui->rewindButton,   SIGNAL(clicked()), this, SIGNAL(rewind()));
    connect(ui->fastForwardButton,  SIGNAL(clicked()), this, SIGNAL(fastForward()));
    connect(ui->previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));
    connect(ui->nextButton,     SIGNAL(clicked()), this, SIGNAL(next()));
}

PlayerControlButtons::~PlayerControlButtons()
{
    delete fileDialog;
    delete ui;
}

void PlayerControlButtons::on_openButton_clicked()
{
    QString filePath;
    filePath = fileDialog->getOpenFileName(this, "Open Module File", QString(), tr("All Modules (*.mod *.xm *.it)"));
    if (!filePath.isEmpty()){
        emit(open(filePath));
    }
}
