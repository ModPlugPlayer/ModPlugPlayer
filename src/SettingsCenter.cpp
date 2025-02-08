/*
SettingsCenter class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SettingsCenter.hpp"
#include "MessageCenter.hpp"

using namespace ModPlugPlayer;

SettingsCenter &ModPlugPlayer::SettingsCenter::getInstance() {
    static SettingsCenter instance;
    return instance;
}

void SettingsCenter::loadSettings() {

}

void SettingsCenter::saveSettings() {

}

MppParameters *SettingsCenter::getParameters() {
    return parameters;
}

SettingsCenter::SettingsCenter(QObject *parent)
    : QObject(parent) {
    connectSignalsAndSlots();
    this->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "ModPlug", "ModPlug Player");
    this->parameters = new MppParameters(settings);
    parameters->load();

    qInfo()<<"Settings file location:"<<settings->fileName();
}

SettingsCenter::~SettingsCenter() {
    parameters->save();
    delete parameters;
    delete settings;
}

void SettingsCenter::onSetupWindowRequested() {
    parameters->save();
}

void SettingsCenter::connectSignalsAndSlots() {

}

void SettingsCenter::onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) {
}

void SettingsCenter::onTitleBarHidingStateChangeRequested(const bool hide) {

}

void SettingsCenter::onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) {
    parameters->snapToViewPort = snapToViewPort;
    emit MessageCenter::getInstance().events.windowEvents.snappingToViewPortStateChanged(snapToViewPort);
}

void SettingsCenter::onSnappingThresholdChangeRequested(const int snappingThreshold) {
    parameters->snappingThreshold= snappingThreshold;
}

void SettingsCenter::onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) {

}

/*
    This block will be spred into the methods of the SettingsCenter.
    parameters->eqEnabled = activated;
    parameters->dspEnabled = activated;
    parameters->amigaFilter = amigaFilter;
    parameters->interpolationFilter = interpolationFilter;
    getParameters()->spectrumAnalyzerWindowFunction = windowFunction;
    parameters->repeatMode = repeatMode;
    playerWindow->setSpectrumAnalyzerWindowFunction(windowFunction);

*/
