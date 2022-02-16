/*
MppParameters class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MPPPARAMETERS_HPP
#define MPPPARAMETERS_HPP
#include "Enums.hpp"
#include <cstdint>
#include <algorithm>
#include <QString>
#include <vector>
#include <QSettings>
#include <DSP.hpp>
#include <QDataStream>
#include <QMetaType>
#ifdef QT_VERSION
#endif

class ParameterBase {
	public:
		ParameterBase();
		~ParameterBase();

		bool isDirty();

		void resetDirtyState();

		QString getName();

		virtual void save(QSettings * settings) = 0;
		virtual void load(QSettings * settings) = 0;

	protected:
		friend class MppParameters;
		bool dirty = false;
		QString name;
};

template<class T>
class Parameter : public ParameterBase{
private:
	friend class MppParameters;
	T value;
public:
	Parameter(const T& value);

	T operator=(const T& value);
	//implicit conversion
	operator T() const;

	void load(QSettings * settings) override;
	void save(QSettings * settings) override;
};

class MppParameters
{
public:
	MppParameters(QSettings *settings);
	void save();
	void load();
    void clearChangedFlags();
    bool isAnyParameterChanged();

    Parameter<int> audioDeviceIndex = -1;
    Parameter<InterpolationFilter> interpolationFilter = InterpolationFilter::Internal;
    Parameter<AmigaFilterType> amigaFilterType = AmigaFilterType::Auto;
    Parameter<int> volume = 0;
	Parameter<int> repeatCount = 0;
	Parameter<int> timeUpdateFrequency = 4;
	Parameter<int> spectrumAnalyzerBarAmount = 0;
	Parameter<bool> alwaysOnTop = false;
	Parameter<bool> hideTitleBar = false;
    Parameter<bool> hideByCloseButton = false;
	Parameter<bool> enableRightClickMenu = false;
	Parameter<bool> enableSystemTray = false;
	Parameter<bool> minimizeToSystemTray = false;
    Parameter<bool> hideApplicationIcon = false;
    Parameter<bool> saveSettingsImmediately = false;
    Parameter<bool> snapToViewPort = true;
    Parameter<int> snapThreshold = 35;
    Parameter<bool> keepStayingInViewPort = true;
    Parameter<RGB> activeTitlebarTextColor = RGB(0,0,0);
	Parameter<RGB> inactiveTitlebarTextColor = RGB(255,255,255);
	Parameter<RGB> activeButtonLightColor = RGB(7,255,52);
    Parameter<RGB> inactiveButtonLightColor = RGB(7,255,255);
	Parameter<RGB> playerBodyTextColor = RGB(0,0,0);
	Parameter<RGB> playerBodyBackgroundColor = RGB(192,192,192);
	Parameter<RGB> lcdDisplayForegroundColor = RGB(0,0,0);
	Parameter<RGB> lcdDisplayBackgroundColor = RGB(188,228,197);
    Parameter<int> spectrumAnalyzerLedAmount = 14;
    Parameter<int> vuMeterLedAmount = 14;

private:
	std::vector<ParameterBase *> parameters;
	void addParameter(ParameterBase &parameter, QString name);
	QSettings * settings;
};

////
template<class T>
Parameter<T>::Parameter(const T& value) {
	this->value = value;
	dirty = false;
}

template<class T>
T Parameter<T>::operator=(const T &value) {
	if(this->value != value) {
		this->value = value;
		dirty = true;
	}
	return this->value;
}

//implicit conversion
template<class T>
Parameter<T>::operator T() const {
	return value;
}

#endif // MPPPARAMETERS_HPP
