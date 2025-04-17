/*
PlayerWindow's startup settings loader method definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "PlayerWindow.hpp"
#include "ui_PlayerWindow.h"
#include <MessageCenter.hpp>
#include "SettingsCenter.hpp"

//ToDo: Move this whole method to SettingsCenter
void PlayerWindow::loadSettings() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    ui->titleBar->setActiveColor(parameters->activeTitlebarTextColor);
    ui->titleBar->setInactiveColor(parameters->inactiveTitlebarTextColor);
    setBodyColor(parameters->playerBodyBackgroundColor, parameters->playerBodyTextColor);
    ui->playerControlButtons->setActiveButtonLightColor(parameters->activeButtonLightColor);
    ui->playerControlButtons->setInactiveButtonLightColor(parameters->inactiveButtonLightColor);
    ui->playerControlButtons->setBackgroundColor(parameters->playerBodyBackgroundColor);
    ui->playerControlButtons->setTextColor(parameters->playerBodyTextColor);
    ui->optionButtons->setActiveButtonLightColor(parameters->activeButtonLightColor);
    ui->optionButtons->setInactiveButtonLightColor(parameters->inactiveButtonLightColor);
    ui->optionButtons->setBackgroundColor(parameters->playerBodyBackgroundColor);
    ui->optionButtons->setTextColor(parameters->playerBodyTextColor);
    ui->lcdPanel->setBackgroundColor(parameters->lcdDisplayBackgroundColor);
    ui->lcdPanel->setTextColor(parameters->lcdDisplayForegroundColor);
    emit MessageCenter::getInstance().requests.soundRequests.outputDeviceChangeRequested(parameters->audioDeviceIndex);
    moveByMouseClick->setSnappingThreshold(parameters->snappingThreshold);
    this->spectrumAlayzerScaleIsLogarithmic = parameters->spectrumAnalyzerScaleIsLogarithmic;

    setSpectrumAnalyzerType(parameters->spectrumAnalyzerType);
    setVuMeterType(parameters->vuMeterType);

    setSpectrumAnalyzerMaximumValue(parameters->spectrumAnalyzerMaximumValue);
    setSpectrumAnalyzerLedAmount(parameters->spectrumAnalyzerLedAmount);
    setSpectrumAnalyzerLedHeightRatio(parameters->spectrumAnalyzerLedHeightRatio);
    setSpectrumAnalyzerBarWidthRatio(parameters->spectrumAnalyzerBarWidthRatio);
    //\Register
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.windowFunctionChangeRequested(parameters->spectrumAnalyzerWindowFunction);
    setSpectrumAnalyzerDimmingRatio(parameters->spectrumAnalyzerDimmingRatio);
    setSpectrumAnalyzerDimmedTransparencyRatio(parameters->spectrumAnalyzerDimmedTransparencyRatio);
    setSpectrumAnalyzerBarAmount(parameters->spectrumAnalyzerBarAmount);
    setSpectrumAnalyzerGradient(parameters->spectrumAnalyzerGradient);

    setVuMeterMaximumValue(parameters->vuMeterMaximumValue);
    setVuMeterMinimumValue(parameters->vuMeterMinimumValue);
    setVuMeterLedAmount(parameters->vuMeterLedAmount);
    setVuMeterLedHeightRatio(parameters->vuMeterLedHeightRatio);
    setVuMeterDimmingRatio(parameters->vuMeterDimmingRatio);
    setVuMeterDimmedTransparencyRatio(parameters->vuMeterDimmedTransparencyRatio);
    setVuMeterGradient(parameters->vuMeterGradient);

    emit MessageCenter::getInstance().requests.windowRequests.keepingStayingInViewPortStateChangeRequested(parameters->keepStayingInViewPort);
    emit MessageCenter::getInstance().requests.windowRequests.snappingToViewPortStateChangeRequested(parameters->snapToViewPort);
    onSnappingThresholdChanged(parameters->snappingThreshold);
    emit MessageCenter::getInstance().requests.windowRequests.alwaysOnTopStateChangeRequested(parameters->alwaysOnTop);
    emit MessageCenter::getInstance().requests.windowRequests.titleBarHidingStateChangeRequested(parameters->hideTitleBar);
    emit MessageCenter::getInstance().requests.moduleRequests.amigaFilterChangeRequested(parameters->amigaFilter);
    emit MessageCenter::getInstance().requests.moduleRequests.interpolationFilterChangeRequested(parameters->interpolationFilter);
    emit MessageCenter::getInstance().requests.songRequests.repeatModeChangeRequested(parameters->repeatMode);
    emit MessageCenter::getInstance().requests.soundRequests.dspStateChangeRequested(parameters->dspEnabled);
    emit MessageCenter::getInstance().requests.soundRequests.eqStateChangeRequested(parameters->eqEnabled);

    SoundResolution soundResolution;
    soundResolution.bitDepth = parameters->bitDepth;
    soundResolution.sampleRate = parameters->samplingFrequency;
    soundResolution.channelMode = parameters->channelMode;
    soundResolution.sampleDataFormat = parameters->sampleDataFormat;
    emit MessageCenter::getInstance().requests.soundRequests.soundResolutionChangeRequested(soundResolution);

    resize(parameters->playerWindowSize);
}
