/*
SpectrumAnalyzerHandler class declarations of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "PlayingCenter.hpp"
#include "SettingsCenter.hpp"
#include <MessageCenter.hpp>
#include <SpectrumAnalyzer.hpp>
#include <SpectrumAnalyzerAnimator.hpp>
#include <QObject>
#include <MessageCenter.hpp>

using namespace ModPlugPlayer;

class SpectrumAnalyzerHandler : public QObject {
    Q_OBJECT
public:
    SpectrumAnalyzerHandler(SpectrumAnalyzer *spectrumAnalyzer);
    double *spectrumData = nullptr;
    void updateSpectrumAnalyzer();

    void loadSettings();
private:
    void initAndConnectTimers();
    void initSpectrumAnalyzer();
    void connectSignalsAndSlots();

    double spectrumAnalyzerTimerTimeoutValue = 0.1;
    PlayingCenter &playingCenter = PlayingCenter::getInstance();
    SettingsCenter &settingsCenter = SettingsCenter::getInstance();
    MessageCenter &messageCenter = MessageCenter::getInstance();
    SpectrumAnalyzer *spectrumAnalyzer;
    QTimer *spectrumAnalyzerTimer = nullptr;
    SpectrumAnalyzerAnimator<double> *spectrumAnalyzerAnimator;
    bool spectrumAlayzerScaleIsLogarithmic = false;
    size_t spectrumAnalyzerBarAmount = 0;
private slots:
    void onBarTypeChangeRequested(BarType barType);
    void onMaximumValueChangeRequested(const int maximumValue);
    void onBarLedAmountChangeRequested(const int ledAmount);
    void onLedHeightRatioChangeRequested(const double ledRatio);
    void onBarWidthRatioChangeRequested(const double barRatio);
    void onDimmingRatioChangeRequested(const double dimmingRatio);
    void onDimmedTransparencyRatioChangeRequested(const double dimmedTransparencyRatio);
    void onBarAmountChangeRequested(const int barAmount);
    void onGradientChangeRequested(const QGradientStops &gradient);
    void onScaleTypeChangeRequested(const bool isLogarithmicScale);
};
