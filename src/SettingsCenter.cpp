/*
SettingsCenter class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SettingsCenter.hpp"
#include "MessageCenter.hpp"
#include "SettingsCenter.hpp"

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
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::alwaysOnTopStateChangeRequested, this, &SettingsCenter::onAlwaysOnTopStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::titleBarHidingStateChangeRequested, this, &SettingsCenter::onTitleBarHidingStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::snappingToViewPortStateChangeRequested, this, &SettingsCenter::onSnappingToViewPortStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::keepingStayingInViewPortStateChangeRequested, this, &SettingsCenter::onKeepingStayingInViewPortStateChangeRequested);

    connect(&MessageCenter::getInstance().events.songEvents, &MessageCenterEvents::SongEvents::repeatModeChanged, this, &SettingsCenter::onRepeatModeChanged);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::eqStateChangeRequested, this, &SettingsCenter::onEqStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::dspStateChangeRequested, this, &SettingsCenter::onDSPStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.moduleRequests, &MessageCenterRequests::ModuleRequests::amigaFilterChangeRequested, this, &SettingsCenter::onAmigaFilterChangeRequested);
    connect(&MessageCenter::getInstance().requests.moduleRequests, &MessageCenterRequests::ModuleRequests::interpolationFilterChangeRequested, this, &SettingsCenter::onInterpolationFilterChangeRequested);
    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::volumeChanged, this, &SettingsCenter::onVolumeChanged);
}

void SettingsCenter::onEqStateChangeRequested(const bool activated) {
    parameters->eqEnabled = activated;
    emit MessageCenter::getInstance().events.soundEvents.eqStateChanged(activated);
}

void SettingsCenter::onDSPStateChangeRequested(const bool activated) {
    parameters->dspEnabled = activated;
    emit MessageCenter::getInstance().events.soundEvents.dspStateChanged(activated);
}

void SettingsCenter::onAmigaFilterChangeRequested(const AmigaFilter amigaFilter) {
    parameters->amigaFilter = amigaFilter;
    emit MessageCenter::getInstance().events.moduleEvents.amigaFilterChanged(amigaFilter);
}

void SettingsCenter::onInterpolationFilterChangeRequested(const InterpolationFilter interpolationFilter) {
    parameters->interpolationFilter = interpolationFilter;
    emit MessageCenter::getInstance().events.moduleEvents.interpolationFilterChanged(interpolationFilter);
}

void SettingsCenter::onVolumeChanged(const int volume) {
    parameters->volume = volume;
}

void SettingsCenter::onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop) {
    parameters->alwaysOnTop = alwaysOnTop;
    emit MessageCenter::getInstance().events.windowEvents.alwaysOnTopStateChanged(alwaysOnTop);
}

void SettingsCenter::onTitleBarHidingStateChangeRequested(const bool hide) {
    parameters->hideTitleBar = hide;
    emit MessageCenter::getInstance().events.windowEvents.titleBarHidingStateChanged(hide);
}

void SettingsCenter::onSnappingToViewPortStateChangeRequested(const bool snapToViewPort) {
    parameters->snapToViewPort = snapToViewPort;
    emit MessageCenter::getInstance().events.windowEvents.snappingToViewPortStateChanged(snapToViewPort);
}

void SettingsCenter::onSnappingThresholdChangeRequested(const int snappingThreshold) {
    parameters->snappingThreshold = snappingThreshold;
    emit MessageCenter::getInstance().events.windowEvents.snappingThresholdChanged(snappingThreshold);
}

void SettingsCenter::onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort) {
    parameters->keepStayingInViewPort = keepStayingInViewPort;    
    emit MessageCenter::getInstance().events.windowEvents.keepingStayingInViewPortStateChanged(keepStayingInViewPort);
}

void SettingsCenter::onRepeatModeChanged(const RepeatMode repeatMode) {
    parameters->repeatMode = repeatMode;
}

/*
    This block will be spred into the methods of the SettingsCenter.
    getParameters()->spectrumAnalyzerWindowFunction = windowFunction;
    playerWindow->setSpectrumAnalyzerWindowFunction(windowFunction);
*/
