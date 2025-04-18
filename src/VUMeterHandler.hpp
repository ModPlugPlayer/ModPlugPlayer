/*
VUMeterHandler class declarations of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include <SpectrumAnalyzer.hpp>
#include <SpectrumAnalyzerAnimator.hpp>
#include <QObject>

class VUMeterHandler : public QObject {
    Q_OBJECT
public:
    VUMeterHandler(SpectrumAnalyzer *vuMeter);
    void loadSettings();

private:
    void initAndConnectTimers();
    void initVuMeter();
    void connectSignalsAndSlots();
    SpectrumAnalyzerAnimator<double> *vuMeterAnimator;
    SpectrumAnalyzer *vuMeter;

private slots:
    void onValueChangeRequested(double vuMeterDbValue);
    void onBarTypeChangeRequested(const BarType barType);
    void onMaximumValueChangeRequested(const int maximumValue);
    void onMinimumValueChangeRequested(const int minimumValue);
    void onBarLedAmountChangeRequested(const int ledAmount);
    void onLedHeightRatioChangeRequested(const double ledRatio);
    void onDimmingRatioChangeRequested(const double dimmingRatio);
    void onDimmedTransparencyRatioChangeRequested(const double dimmedTransparencyRatio);
    void onGradientChangeRequested(const QGradientStops &gradient);
};
