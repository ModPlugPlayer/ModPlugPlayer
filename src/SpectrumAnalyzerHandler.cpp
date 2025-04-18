/*
SpectrumAnalyzerHandler class definitions of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SpectrumAnalyzerHandler.hpp"

SpectrumAnalyzerHandler::SpectrumAnalyzerHandler(SpectrumAnalyzer *spectrumAnalyzer) {
    this->spectrumAnalyzer = spectrumAnalyzer;
    this->spectrumAnalyzerAnimator = new SpectrumAnalyzerAnimator<double>(20, 0, SettingsCenter::getInstance().getParameters()->spectrumAnalyzerMaximumValue);


    MotionProperties<double> rs, fs, rv, fv;
    rs.acceleration = -10000;
    rs.motionType = MotionType::ConstantAcceleration;

    fs.acceleration = -10000;
    fs.motionType = MotionType::ConstantAcceleration;

    rv.acceleration = -2500;
    fv.acceleration = -2500;
    rv.motionType = MotionType::ConstantAcceleration;
    fv.motionType = MotionType::ConstantAcceleration;


    spectrumAnalyzerAnimator->setFallingMotionProperties(fs);
    spectrumAnalyzerAnimator->setRaisingMotionProperties(rs);
    spectrumAnalyzerAnimator->start();
    loadSettings();
    initSpectrumAnalyzer();
    initAndConnectTimers();
    updateSpectrumAnalyzer();
}

void SpectrumAnalyzerHandler::loadSettings() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();

    this->spectrumAlayzerScaleIsLogarithmic = parameters->spectrumAnalyzerScaleIsLogarithmic;

    onBarTypeChangeRequested(parameters->spectrumAnalyzerType);

    onMaximumValueChangeRequested(parameters->spectrumAnalyzerMaximumValue);
    onBarLedAmountChangeRequested(parameters->spectrumAnalyzerLedAmount);
    onLedHeightRatioChangeRequested(parameters->spectrumAnalyzerLedHeightRatio);
    onBarWidthRatioChangeRequested(parameters->spectrumAnalyzerBarWidthRatio);
    //\Register
    emit MessageCenter::getInstance().requests.spectrumAnalyzerRequests.windowFunctionChangeRequested(parameters->spectrumAnalyzerWindowFunction);
    onDimmingRatioChangeRequested(parameters->spectrumAnalyzerDimmingRatio);
    onDimmedTransparencyRatioChangeRequested(parameters->spectrumAnalyzerDimmedTransparencyRatio);
    onBarAmountChangeRequested(parameters->spectrumAnalyzerBarAmount);
    onGradientChangeRequested(parameters->spectrumAnalyzerGradient);
}

void SpectrumAnalyzerHandler::initSpectrumAnalyzer() {
    MppParameters *parameters = SettingsCenter::getInstance().getParameters();
    SpectrumAnalyzerParameters spectrumAnalyzerParameters;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumData = new double[spectrumAnalyzerParameters.barAmount];

    std::fill(spectrumData, spectrumData + spectrumAnalyzerParameters.barAmount, 0);

    spectrumAnalyzerParameters.barDirection = Qt::Orientation::Vertical;
    spectrumAnalyzerParameters.peakValue = parameters->spectrumAnalyzerMaximumValue;
    spectrumAnalyzerParameters.barWidthRatio = parameters->spectrumAnalyzerBarWidthRatio;
    spectrumAnalyzerParameters.dimmingRatio = parameters->spectrumAnalyzerDimmingRatio*100;
    spectrumAnalyzerParameters.dimmedTransparencyRatio = parameters->spectrumAnalyzerDimmedTransparencyRatio*100;
    spectrumAnalyzerParameters.discreteParameters.ledHeightRatio = parameters->spectrumAnalyzerLedHeightRatio;;
    spectrumAnalyzerParameters.discreteParameters.barLedAmount = parameters->spectrumAnalyzerLedAmount;
    spectrumAnalyzerParameters.barAmount = parameters->spectrumAnalyzerBarAmount;
    spectrumAnalyzerParameters.gradientStops = parameters->spectrumAnalyzerGradient;

    spectrumAnalyzer->setParameters(spectrumAnalyzerParameters);
}

void SpectrumAnalyzerHandler::connectSignalsAndSlots() {
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::barTypeChangeRequested, this, &SpectrumAnalyzerHandler::onBarTypeChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::barLedAmountChangeRequested, this, &SpectrumAnalyzerHandler::onBarLedAmountChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::barWidthRatioChangeRequested, this, &SpectrumAnalyzerHandler::onBarWidthRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::ledHeightRatioChangeRequested, this, &SpectrumAnalyzerHandler::onLedHeightRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::barAmountChangeRequested, this, &SpectrumAnalyzerHandler::onBarAmountChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::dimmingRatioChangeRequested, this, &SpectrumAnalyzerHandler::onDimmingRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::dimmedTransparencyRatioChangeRequested, this, &SpectrumAnalyzerHandler::onDimmedTransparencyRatioChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::maximumValueChangeRequested, this, &SpectrumAnalyzerHandler::onMaximumValueChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::gradientChangeRequested, this, &SpectrumAnalyzerHandler::onGradientChangeRequested);
    connect(&MessageCenter::getInstance().requests.spectrumAnalyzerRequests, &MessageCenterRequests::BarDisplayRequests::scaleTypeChangeRequested, this, &SpectrumAnalyzerHandler::onScaleTypeChangeRequested);
}

void SpectrumAnalyzerHandler::updateSpectrumAnalyzer() {
    MppParameters *parameters = settingsCenter.getParameters();
    playingCenter.getSpectrumData(spectrumData);
    if(spectrumAlayzerScaleIsLogarithmic) {
        DSP::DSP<double>::magnitudeToDecibel(spectrumData, spectrumData, spectrumAnalyzerBarAmount);
    }
    spectrumAnalyzerAnimator->setValues(spectrumData);
    spectrumAnalyzerAnimator->getValues(spectrumData);
    //float volumeCoefficient = double(ui->volumeControl->value())/100;
    double vuMeterDbValue = playingCenter.getVUMeterValue();
    if(vuMeterDbValue == NAN)
        vuMeterDbValue = parameters->vuMeterMinimumValue;
    else if(vuMeterDbValue < parameters->vuMeterMinimumValue)
        vuMeterDbValue = parameters->vuMeterMinimumValue;
    else if(vuMeterDbValue > parameters->vuMeterMaximumValue)
        vuMeterDbValue = parameters->vuMeterMaximumValue;
    emit messageCenter.requests.vuMeterRequests.valueChangeRequested(vuMeterDbValue);

    for(int i=0; i<20; i++) {
        double barValue = spectrumData[i];
        if(barValue == NAN)
            barValue = 0;
        else if(barValue < 0)
            barValue = 0;
        else if(barValue > parameters->spectrumAnalyzerMaximumValue)
            barValue = parameters->spectrumAnalyzerMaximumValue;
        spectrumAnalyzer->setBarValue(i, barValue);
    }

    spectrumAnalyzer->update();
}

void SpectrumAnalyzerHandler::onBarTypeChangeRequested(const BarType barType) {
    spectrumAnalyzer->setBarType(barType);
}

void SpectrumAnalyzerHandler::onMaximumValueChangeRequested(const int maximumValue) {
    spectrumAnalyzer->setPeakValue(maximumValue);
    spectrumAnalyzerAnimator->setMaxValue(maximumValue);
}

void SpectrumAnalyzerHandler::onBarLedAmountChangeRequested(const int ledAmount) {
    spectrumAnalyzer->setLedAmount(ledAmount);
}

void SpectrumAnalyzerHandler::onLedHeightRatioChangeRequested(const double ledRatio) {
    spectrumAnalyzer->setLedHeightRatio(ledRatio);
}

void SpectrumAnalyzerHandler::onBarWidthRatioChangeRequested(const double barRatio) {
    spectrumAnalyzer->setBarWidthRatio(barRatio);
}

void SpectrumAnalyzerHandler::onDimmingRatioChangeRequested(const double dimmingRatio) {
    spectrumAnalyzer->setDimmingRatio(dimmingRatio);
}

void SpectrumAnalyzerHandler::onDimmedTransparencyRatioChangeRequested(const double dimmedTransparencyRatio) {
    spectrumAnalyzer->setDimmedTransparencyRatio(dimmedTransparencyRatio);
}

void SpectrumAnalyzerHandler::onBarAmountChangeRequested(const int barAmount) {
    spectrumAnalyzerBarAmount = barAmount;
    spectrumAnalyzer->setBarAmount(barAmount);
}

void SpectrumAnalyzerHandler::onGradientChangeRequested(const QGradientStops & gradient) {
    spectrumAnalyzer->setGradient(gradient);
}

void SpectrumAnalyzerHandler::onScaleTypeChangeRequested(const bool isLogarithmicScale) {
    this->spectrumAlayzerScaleIsLogarithmic = isLogarithmicScale;
    MppParameters *parameters = settingsCenter.getParameters();
    parameters->spectrumAnalyzerScaleIsLogarithmic = isLogarithmicScale; //Move this line into SettingsCenter and remove the line above
}

void SpectrumAnalyzerHandler::initAndConnectTimers() {
    spectrumAnalyzerTimer = new QTimer(this);
    spectrumAnalyzerTimer->setInterval(spectrumAnalyzerTimerTimeoutValue);
    spectrumAnalyzerTimer->start();
    connect(spectrumAnalyzerTimer, &QTimer::timeout, this, &SpectrumAnalyzerHandler::updateSpectrumAnalyzer);
}
