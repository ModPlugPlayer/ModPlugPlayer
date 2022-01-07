#include "SystemCaptionButtons.hpp"
#include "ui_SystemCaptionButtons.h"

SystemCaptionButtons::SystemCaptionButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemCaptionButtons)
{
    ui->setupUi(this);
    QObject::connect(ui->minimizeButton, &QPushButton::clicked, this, &SystemCaptionButtons::minimizeButtonClicked);
    QObject::connect(ui->miniPlayerButton, &QPushButton::clicked, this, &SystemCaptionButtons::miniPlayerButtonClicked);
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &SystemCaptionButtons::closeButtonClicked);
}

SystemCaptionButtons::~SystemCaptionButtons()
{
    delete ui;
}
