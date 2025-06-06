/*
AboutWindow class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "AboutWindow.hpp"
#include "ui_AboutWindow.h"
#include <QLayout>
#include "VersionInfoWindow.hpp"
#include "VersionController.hpp"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    ui->versionInfo->setText(QString::fromStdString(VersionController::getVersionInfo().prettyString()));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_pushButtonVersionInfo_clicked()
{
    VersionInfoWindow versionInfoWindow(this);
    versionInfoWindow.exec();
}


void AboutWindow::on_pushButtonOk_clicked()
{
    close();
}

