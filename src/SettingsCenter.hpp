/*
SettingsCenter class declarations of ModPlug Player
Copyright (C) 2025 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QObject>
#include "MppParameters.hpp"

namespace ModPlugPlayer {
    class SettingsCenter : public QObject {
        Q_OBJECT
    public:
        explicit SettingsCenter(QObject *parent = nullptr);
        SettingsCenter(SettingsCenter const &) = delete;
        void operator =(SettingsCenter const &) = delete;
        static SettingsCenter& getInstance();
        void loadSettings();
        void saveSettings();
        MppParameters * getParameters();
        ~SettingsCenter();
    private:
        MppParameters *parameters = nullptr;
        QSettings *settings = nullptr;
        void connectSignalsAndSlots();
        static std::mutex mutex;
    private slots:
        void onSettingsSaveRequested();
        void onSettingsSavingImmediatelyModeChanged(bool saveImmediately);
        void onSetupWindowRequested();
        void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop);
        void onTitleBarHidingStateChangeRequested(const bool hide);
        void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort);
        void onSnappingThresholdChangeRequested(const int snappingThreshold);
        void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort);
        void onRepeatModeChanged(const RepeatMode repeatMode);
        void onEqStateChangeRequested(const bool activated);
        void onDSPStateChangeRequested(const bool activated);
        void onAmigaFilterChanged(const AmigaFilter amigaFilter);
        void onInterpolationFilterChanged(const InterpolationFilter interpolationFilter);
        void onVolumeChanged(const int volume);
        void onSoundResolutionChanged(const SoundResolution soundResolution);

        void onVUMeterBarTypeChanged(const BarType barType);
        void onVUMeterMaximumValueChanged(const int maximumValue);
        void onVUMeterMinimumValueChanged(const int minimumValue);
        void onVUMeterBarLedAmountChanged(const int ledAmount);
        void onVUMeterLedHeightRatioChanged(const double ledRatio);
        void onVUMeterDimmingRatioChanged(const double dimmingRatio);
        void onVUMeterDimmedTransparencyRatioChanged(const double dimmedTransparencyRatio);
        void onVUMeterGradientChanged(const QGradientStops &gradient);

        void onSpectrumAnalyzerBarTypeChanged(BarType barType);
        void onSpectrumAnalyzerMaximumValueChanged(const int maximumValue);
        void onSpectrumAnalyzerBarLedAmountChanged(const int ledAmount);
        void onSpectrumAnalyzerLedHeightRatioChanged(const double ledRatio);
        void onSpectrumAnalyzerBarWidthRatioChanged(const double barRatio);
        void onSpectrumAnalyzerDimmingRatioChanged(const double dimmingRatio);
        void onSpectrumAnalyzerDimmedTransparencyRatioChanged(const double dimmedTransparencyRatio);
        void onSpectrumAnalyzerBarAmountChanged(const int barAmount);
        void onSpectrumAnalyzerGradientChanged(const QGradientStops &gradient);
        void onSpectrumAnalyzerScaleTypeChanged(const bool isLogarithmicScale);
        void onSpectrumAnalyzerWindowFunctionChanged(const WindowFunction windowFunction);
    };
}
