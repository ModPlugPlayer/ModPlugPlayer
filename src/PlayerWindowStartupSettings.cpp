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
    moduleHandler.setOutputDeviceIndex(parameters->audioDeviceIndex);
    moveByMouseClick->setSnappingThreshold(parameters->snappingThreshold);
    this->spectrumAlayzerScaleIsLogarithmic = parameters->spectrumAnalyzerScaleIsLogarithmic;

    setSpectrumAnalyzerType(parameters->spectrumAnalyzerType);
    setVuMeterType(parameters->vuMeterType);

    setSpectrumAnalyzerMaximumValue(parameters->spectrumAnalyzerMaximumValue);
    setSpectrumAnalyzerLedAmount(parameters->spectrumAnalyzerLedAmount);
    setSpectrumAnalyzerLedHeightRatio(parameters->spectrumAnalyzerLedHeightRatio);
    setSpectrumAnalyzerBarWidthRatio(parameters->spectrumAnalyzerBarWidthRatio);
    setSpectrumAnalyzerWindowFunction(parameters->spectrumAnalyzerWindowFunction);
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

    emit MessageCenter::getInstance().keepingStayingInViewPortStateChangeRequested(parameters->keepStayingInViewPort);
    emit MessageCenter::getInstance().snappingToViewPortStateChangeRequested(parameters->snapToViewPort);
    onSnappingThresholdChangeRequested(parameters->snappingThreshold);
    emit MessageCenter::getInstance().alwaysOnTopStateChangeRequested(parameters->alwaysOnTop);
    emit MessageCenter::getInstance().titleBarHidingStateChangeRequested(parameters->hideTitleBar);
    emit amigaFilterChangeRequested(parameters->amigaFilter);
    emit interpolationFilterChangeRequested(parameters->interpolationFilter);
    emit MessageCenter::getInstance().repeatModeChangeRequested(parameters->repeatMode);
    emit MessageCenter::getInstance().dspStateChangeRequested(parameters->dspEnabled);
    emit MessageCenter::getInstance().eqStateChangeRequested(parameters->eqEnabled);
    resize(parameters->playerWindowSize);
}
