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
    connect(&MessageCenter::getInstance().requests.vuMeterRequests, qOverload<double>(&MessageCenterRequests::BarDisplayRequests::valueChangeRequested), this, qOverload<double>(&VUMeterHandler::onVUMeterValueChangeRequested));
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

void VUMeterHandler::setVuMeterType(BarType barType) {
    vuMeter->setBarType(barType);
}

void VUMeterHandler::setVuMeterMaximumValue(int maximumValue) {
    vuMeter->setPeakValue(maximumValue);
    vuMeterAnimator->setMaxValue(maximumValue);
}

void VUMeterHandler::setVuMeterMinimumValue(int minimumValue) {
    vuMeter->setFloorValue(minimumValue);
    vuMeterAnimator->setMinValue(minimumValue);
}

void VUMeterHandler::setVuMeterLedAmount(int ledAmount) {
    vuMeter->setLedAmount(ledAmount);
}

void VUMeterHandler::setVuMeterLedHeightRatio(double ledRatio) {
    vuMeter->setLedHeightRatio(ledRatio);
}

void VUMeterHandler::setVuMeterDimmingRatio(double dimmingRatio) {
    vuMeter->setDimmingRatio(dimmingRatio);
}

void VUMeterHandler::setVuMeterDimmedTransparencyRatio(double dimmedTransparencyRatio) {
    vuMeter->setDimmedTransparencyRatio(dimmedTransparencyRatio);
}

void VUMeterHandler::setVuMeterGradient(const QGradientStops & gradient) {
    vuMeter->setGradient(gradient);
}

void VUMeterHandler::loadSettings() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();

    setVuMeterType(parameters->vuMeterType);
    setVuMeterMaximumValue(parameters->vuMeterMaximumValue);
    setVuMeterMinimumValue(parameters->vuMeterMinimumValue);
    setVuMeterLedAmount(parameters->vuMeterLedAmount);
    setVuMeterLedHeightRatio(parameters->vuMeterLedHeightRatio);
    setVuMeterDimmingRatio(parameters->vuMeterDimmingRatio);
    setVuMeterDimmedTransparencyRatio(parameters->vuMeterDimmedTransparencyRatio);
    setVuMeterGradient(parameters->vuMeterGradient);
}

void VUMeterHandler::onVUMeterValueChangeRequested(double vuMeterDbValue) {
    vuMeterAnimator->setValues(&vuMeterDbValue);
    vuMeterAnimator->getValues(&vuMeterDbValue);
    vuMeter->setBarValue(0, vuMeterDbValue);
    vuMeter->update();
}
