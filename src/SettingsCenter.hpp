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
    class SettingsCenter : public QObject
    {
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
        void onSetupWindowRequested();
        void connectSignalsAndSlots();
        static std::mutex mutex;
    private slots:
        void onAlwaysOnTopStateChangeRequested(const bool alwaysOnTop);
        void onTitleBarHidingStateChangeRequested(const bool hide);
        void onSnappingToViewPortStateChangeRequested(const bool snapToViewPort);
        void onSnappingThresholdChangeRequested(const int snappingThreshold);
        void onKeepingStayingInViewPortStateChangeRequested(const bool keepStayingInViewPort);
    };
}
