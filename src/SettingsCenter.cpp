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
    connect(&MessageCenter::getInstance().requests.settingsRequests, &MessageCenterRequests::SettingsRequests::settingsSaveRequested, this, &SettingsCenter::onSettingsSaveRequested);
    connect(&MessageCenter::getInstance().events.settingsEvents, &MessageCenterEvents::SettingsEvents::settingsSavingImmediatelyModeChanged, this, &SettingsCenter::onSettingsSavingImmediatelyModeChanged);

    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::alwaysOnTopStateChangeRequested, this, &SettingsCenter::onAlwaysOnTopStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::titleBarHidingStateChangeRequested, this, &SettingsCenter::onTitleBarHidingStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::snappingToViewPortStateChangeRequested, this, &SettingsCenter::onSnappingToViewPortStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.windowRequests, &MessageCenterRequests::WindowRequests::keepingStayingInViewPortStateChangeRequested, this, &SettingsCenter::onKeepingStayingInViewPortStateChangeRequested);

    connect(&MessageCenter::getInstance().events.songEvents, &MessageCenterEvents::SongEvents::repeatModeChanged, this, &SettingsCenter::onRepeatModeChanged);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::eqStateChangeRequested, this, &SettingsCenter::onEqStateChangeRequested);
    connect(&MessageCenter::getInstance().requests.soundRequests, &MessageCenterRequests::SoundRequests::dspStateChangeRequested, this, &SettingsCenter::onDSPStateChangeRequested);
    connect(&MessageCenter::getInstance().events.moduleEvents, &MessageCenterEvents::ModuleEvents::amigaFilterChanged, this, &SettingsCenter::onAmigaFilterChanged);
    connect(&MessageCenter::getInstance().events.moduleEvents, &MessageCenterEvents::ModuleEvents::interpolationFilterChanged, this, &SettingsCenter::onInterpolationFilterChanged);
    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::volumeChanged, this, &SettingsCenter::onVolumeChanged);
    connect(&MessageCenter::getInstance().events.soundEvents, &MessageCenterEvents::SoundEvents::soundResolutionChanged, this, &SettingsCenter::onSoundResolutionChanged);

    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::barTypeChanged, this, &SettingsCenter::onSpectrumAnalyzerBarTypeChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::barLedAmountChanged, this, &SettingsCenter::onSpectrumAnalyzerBarLedAmountChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::barWidthRatioChanged, this, &SettingsCenter::onSpectrumAnalyzerBarWidthRatioChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::ledHeightRatioChanged, this, &SettingsCenter::onSpectrumAnalyzerLedHeightRatioChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::barAmountChanged, this, &SettingsCenter::onSpectrumAnalyzerBarAmountChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::dimmingRatioChanged, this, &SettingsCenter::onSpectrumAnalyzerDimmingRatioChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::dimmedTransparencyRatioChanged, this, &SettingsCenter::onSpectrumAnalyzerDimmedTransparencyRatioChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::maximumValueChanged, this, &SettingsCenter::onSpectrumAnalyzerMaximumValueChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::gradientChanged, this, &SettingsCenter::onSpectrumAnalyzerGradientChanged);
    connect(&MessageCenter::getInstance().events.spectrumAnalyzerEvents, &MessageCenterEvents::BarDisplayEvents::scaleTypeChanged, this, &SettingsCenter::onSpectrumAnalyzerScaleTypeChanged);

    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::barTypeChanged, this, &SettingsCenter::onVUMeterBarTypeChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::maximumValueChanged, this, &SettingsCenter::onVUMeterMaximumValueChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::minimumValueChanged, this, &SettingsCenter::onVUMeterMinimumValueChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::barLedAmountChanged, this, &SettingsCenter::onVUMeterBarLedAmountChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::ledHeightRatioChanged, this, &SettingsCenter::onVUMeterLedHeightRatioChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::dimmingRatioChanged, this, &SettingsCenter::onVUMeterDimmingRatioChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::dimmedTransparencyRatioChanged, this, &SettingsCenter::onVUMeterDimmedTransparencyRatioChanged);
    connect(&MessageCenter::getInstance().events.vuMeterEvents, &MessageCenterEvents::BarDisplayEvents::gradientChanged, this, &SettingsCenter::onVUMeterGradientChanged);
}

void SettingsCenter::onSettingsSaveRequested() {
    parameters->save();
}

void SettingsCenter::onSettingsSavingImmediatelyModeChanged(bool saveImmediately) {
    parameters->saveSettingsImmediately = saveImmediately;
    emit MessageCenter::getInstance().events.settingsEvents.settingsSavingImmediatelyModeChanged(saveImmediately);
}

void SettingsCenter::onEqStateChangeRequested(const bool activated) {
    parameters->eqEnabled = activated;
    emit MessageCenter::getInstance().events.soundEvents.eqStateChanged(activated);
}

void SettingsCenter::onDSPStateChangeRequested(const bool activated) {
    parameters->dspEnabled = activated;
    emit MessageCenter::getInstance().events.soundEvents.dspStateChanged(activated);
}

void SettingsCenter::onAmigaFilterChanged(const AmigaFilter amigaFilter) {
    parameters->amigaFilter = amigaFilter;
}

void SettingsCenter::onInterpolationFilterChanged(const InterpolationFilter interpolationFilter) {
    parameters->interpolationFilter = interpolationFilter;
}

void SettingsCenter::onVolumeChanged(const int volume) {
    parameters->volume = volume;
}

void SettingsCenter::onSoundResolutionChanged(const SoundResolution soundResolution) {
    parameters->bitDepth = soundResolution.bitDepth;
    parameters->samplingFrequency = soundResolution.sampleRate;
    parameters->channelMode = soundResolution.channelMode;
    parameters->sampleDataFormat = soundResolution.sampleDataFormat;
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

void SettingsCenter::onVUMeterBarTypeChanged(const BarType barType) {
    parameters->vuMeterType = barType;
}

void SettingsCenter::onVUMeterMaximumValueChanged(const int maximumValue) {}

void SettingsCenter::onVUMeterMinimumValueChanged(const int minimumValue) {}

void SettingsCenter::onVUMeterBarLedAmountChanged(const int ledAmount) {}

void SettingsCenter::onVUMeterLedHeightRatioChanged(const double ledRatio) {}

void SettingsCenter::onVUMeterDimmingRatioChanged(const double dimmingRatio) {}

void SettingsCenter::onVUMeterDimmedTransparencyRatioChanged(const double dimmedTransparencyRatio) {}

void SettingsCenter::onVUMeterGradientChanged(const QGradientStops &gradient) {}

void SettingsCenter::onSpectrumAnalyzerBarTypeChanged(BarType barType) {}

void SettingsCenter::onSpectrumAnalyzerMaximumValueChanged(const int maximumValue) {}

void SettingsCenter::onSpectrumAnalyzerBarLedAmountChanged(const int ledAmount) {}

void SettingsCenter::onSpectrumAnalyzerLedHeightRatioChanged(const double ledRatio) {}

void SettingsCenter::onSpectrumAnalyzerBarWidthRatioChanged(const double barRatio) {}

void SettingsCenter::onSpectrumAnalyzerDimmingRatioChanged(const double dimmingRatio) {}

void SettingsCenter::onSpectrumAnalyzerDimmedTransparencyRatioChanged(const double dimmedTransparencyRatio) {}

void SettingsCenter::onSpectrumAnalyzerBarAmountChanged(const int barAmount) {}

void SettingsCenter::onSpectrumAnalyzerGradientChanged(const QGradientStops &gradient) {}

void SettingsCenter::onSpectrumAnalyzerScaleTypeChanged(const bool isLogarithmicScale) {
    parameters->spectrumAnalyzerScaleIsLogarithmic = isLogarithmicScale; //Move this line into SettingsCenter and remove the line above
}

void SettingsCenter::onSpectrumAnalyzerWindowFunctionChanged(const WindowFunction windowFunction) {
    parameters->spectrumAnalyzerWindowFunction = windowFunction;
}
