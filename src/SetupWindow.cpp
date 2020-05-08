#include "SetupWindow.hpp"
#include "ui_SetupWindow.h"

SetupWindow::SetupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupWindow)
{
    ui->setupUi(this);
}

SetupWindow::~SetupWindow()
{
    delete ui;
}

void SetupWindow::on_listWidget_currentRowChanged(int currentRow)
{
    ui->pages->setCurrentIndex(currentRow);
}
