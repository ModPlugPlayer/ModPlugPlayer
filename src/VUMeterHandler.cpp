/*
VUMeterHandler class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "VUMeterHandler.hpp"
#include <SettingsCenter.hpp>
#include <MessageCenter.hpp>

VUMeterHandler::VUMeterHandler(SpectrumAnalyzer *vuMeter) {
    this->vuMeter = vuMeter;
    this->vuMeterAnimator = new SpectrumAnalyzerAnimator<double>(1, -40, -8);


    MotionProperties<double> rs, fs, rv, fv;
    rs.acceleration = -10000;
    rs.motionType = MotionType::ConstantAcceleration;

    fs.acceleration = -10000;
    fs.motionType = MotionType::ConstantAcceleration;

    rv.acceleration = -2500;
    fv.acceleration = -2500;
    rv.motionType = MotionType::ConstantAcceleration;
    fv.motionType = MotionType::ConstantAcceleration;

    vuMeterAnimator->setFallingMotionProperties(fv);
    vuMeterAnimator->setRaisingMotionProperties(rv);
    vuMeterAnimator->start();

    initVuMeter();
    loadSettings();
    connectSignalsAndSlots();
}

void VUMeterHandler::initVuMeter() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    SpectrumAnalyzerParameters vuMeterParameters;

    vuMeterParameters.barDirection = Qt::Orientation::Vertical;
    vuMeterParameters.barAmount = 1;

    vuMeterParameters.peakValue = parameters->vuMeterMaximumValue;
    vuMeterParameters.floorValue = parameters->vuMeterMinimumValue;
    vuMeterParameters.barWidthRatio = 1;
    vuMeterParameters.dimmingRatio = parameters->vuMeterDimmingRatio*100;
    vuMeterParameters.dimmedTransparencyRatio = parameters->vuMeterDimmedTransparencyRatio*100;
    vuMeterParameters.discreteParameters.ledHeightRatio = parameters->vuMeterLedHeightRatio;;
    vuMeterParameters.discreteParameters.barLedAmount = parameters->vuMeterLedAmount;
    vuMeterParameters.gradientStops = parameters->vuMeterGradient;
    vuMeter->setParameters(vuMeterParameters);
}

void VUMeterHandler::connectSignalsAndSlots() {
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, qOverload<double>(&MessageCenterRequests::BarDisplayRequests::valueChangeRequested), this, qOverload<double>(&VUMeterHandler::onValueChangeRequested));
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::barTypeChangeRequested, this, &VUMeterHandler::onBarTypeChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::maximumValueChangeRequested, this, &VUMeterHandler::onMaximumValueChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::minimumValueChangeRequested, this, &VUMeterHandler::onMinimumValueChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::barLedAmountChangeRequested, this, &VUMeterHandler::onBarLedAmountChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::ledHeightRatioChangeRequested, this, &VUMeterHandler::onLedHeightRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::dimmingRatioChangeRequested, this, &VUMeterHandler::onDimmingRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::dimmedTransparencyRatioChangeRequested, this, &VUMeterHandler::onDimmedTransparencyRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, &MessageCenterRequests::BarDisplayRequests::gradientChangeRequested, this, &VUMeterHandler::onGradientChangeRequested);
}

void VUMeterHandler::onBarTypeChangeRequested(const BarType barType) {
    vuMeter->setBarType(barType);
}

void VUMeterHandler::onMaximumValueChangeRequested(const int maximumValue) {
    vuMeter->setPeakValue(maximumValue);
    vuMeterAnimator->setMaxValue(maximumValue);
}

void VUMeterHandler::onMinimumValueChangeRequested(const int minimumValue) {
    vuMeter->setFloorValue(minimumValue);
    vuMeterAnimator->setMinValue(minimumValue);
}

void VUMeterHandler::onBarLedAmountChangeRequested(const int ledAmount) {
    vuMeter->setLedAmount(ledAmount);
}

void VUMeterHandler::onLedHeightRatioChangeRequested(const double ledRatio) {
    vuMeter->setLedHeightRatio(ledRatio);
}

void VUMeterHandler::onDimmingRatioChangeRequested(const double dimmingRatio) {
    vuMeter->setDimmingRatio(dimmingRatio);
}

void VUMeterHandler::onDimmedTransparencyRatioChangeRequested(const double dimmedTransparencyRatio) {
    vuMeter->setDimmedTransparencyRatio(dimmedTransparencyRatio);
}

void VUMeterHandler::onGradientChangeRequested(const QGradientStops & gradient) {
    vuMeter->setGradient(gradient);
}

void VUMeterHandler::loadSettings() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();

    onBarTypeChangeRequested(parameters->vuMeterType);
    onMaximumValueChangeRequested(parameters->vuMeterMaximumValue);
    onMinimumValueChangeRequested(parameters->vuMeterMinimumValue);
    onBarLedAmountChangeRequested(parameters->vuMeterLedAmount);
    onLedHeightRatioChangeRequested(parameters->vuMeterLedHeightRatio);
    onDimmingRatioChangeRequested(parameters->vuMeterDimmingRatio);
    onDimmedTransparencyRatioChangeRequested(parameters->vuMeterDimmedTransparencyRatio);
    onGradientChangeRequested(parameters->vuMeterGradient);
}

void VUMeterHandler::onValueChangeRequested(double vuMeterDbValue) {
    vuMeterAnimator->setValues(&vuMeterDbValue);
    vuMeterAnimator->getValues(&vuMeterDbValue);
    vuMeter->setBarValue(0, vuMeterDbValue);
    vuMeter->update();
}
