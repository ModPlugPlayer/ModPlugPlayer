/*
MppParameters class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "MppParameters.hpp"
#include <QMetaType>

bool ParameterBase::isDirty(){
	return dirty;
}

void ParameterBase::resetDirtyState(){
	this->dirty = false;
}

QString ParameterBase::getName() {
	return name;
}

MppParameters::MppParameters(QSettings *settings)
{
    //qRegisterMetaTypeStreamOperators<RGB>("RGB");

	this->settings = settings;

    addParameter(audioDeviceIndex, "AudioDeviceIndex");
	addParameter(interpolationFilter, "InterpolationFilter");
    addParameter(amigaFilterType, "AmigaFilterType");
    addParameter(volume, "Volume");
	addParameter(repeatCount, "RepeatCount");
    addParameter(timeUpdateFrequency, "TimeUpdateFrequency");
    addParameter(alwaysOnTop, "AlwaysOnTop");
	addParameter(activeTitlebarTextColor, "ActiveTitleBarTextColor");
	addParameter(inactiveTitlebarTextColor, "InactiveTitleBarTextColor");
	addParameter(activeButtonLightColor, "ActiveButtonLightColor");
	addParameter(inactiveButtonLightColor, "InactiveButtonLightColor");
	addParameter(playerBodyTextColor, "PlayerBodyTextColor");
	addParameter(playerBodyBackgroundColor, "PlayerBodyBackgroundColor");
	addParameter(lcdDisplayForegroundColor, "LCD-DisplayForegroundColor");
	addParameter(lcdDisplayBackgroundColor, "LCD-DisplayBackgroundColor");
    addParameter(hideByCloseButton, "HideByCloseButton");
    addParameter(saveSettingsImmediately, "SaveSettingsImmediately");
    addParameter(snapToViewPort, "SnapToViewPort");
    addParameter(snappingThreshold, "SnappingThreshold");
    addParameter(keepStayingInViewPort, "KeepStayingInViewPort");
    addParameter(enableSystemTray, "EnableSystemTray");
    addParameter(hideTitleBar, "HideTitleBar");

    addParameter(spectrumAnalyzerType, "SpectrumAnalyzerType");
    addParameter(spectrumAnalyzerBarAmount, "SpectrumAnalyzerBarAmount");
    addParameter(spectrumAnalyzerLedAmount, "SpectrumAnalyzerLEDAmount");
    addParameter(spectrumAnalyzerLedRatio, "SpectrumAnalyzerLEDRatio");
    addParameter(spectrumAnalyzerBarRatio, "SpectrumAnalyzerBarRatio");
    addParameter(spectrumAnalyzerTransparencyRatio, "SpectrumAnalyzerTransparencyRatio");
    addParameter(spectrumAnalyzerDimmingRatio, "SpectrumAnalyzerDimmingRatio");

    addParameter(vuMeterType, "VUMeterType");
    addParameter(vuMeterLedAmount, "VUMeterLEDAmount");
    addParameter(vuMeterLedRatio, "VUMeterLEDRatio");
    addParameter(vuMeterBarRatio, "VUMeterBarRatio");
    addParameter(vuMeterTransparencyRatio, "VUMeterTransparencyRatio");
    addParameter(vuMeterDimmingRatio, "VUMeterDimmingRatio");
}

void MppParameters::save()
{
	for(ParameterBase *parameter:parameters) {
		if(parameter->isDirty())
			parameter->save(settings);
	}
	settings->sync();
}

void MppParameters::load()
{
	for(ParameterBase *parameter:parameters) {
		parameter->load(settings);
	}
}

void MppParameters::clearChangedFlags(){
	for(ParameterBase *parameter:parameters) {
		parameter->resetDirtyState();
	}
}

bool MppParameters::isAnyParameterChanged(){
	for(ParameterBase *parameter:parameters) {
		if(parameter->isDirty())
			return true;
	}
	return false;
}

void MppParameters::addParameter(ParameterBase &parameter, QString name)
{
	parameter.name = name;
	parameters.push_back(&parameter);
}

template<class T>
void Parameter<T>::load(QSettings * settings)
{
	QVariant value = settings->value(name, QVariant::fromValue(this->value));
	if(!value.isNull())
		this->value = value.value<T>();
}

template<class T>
void Parameter<T>::save(QSettings * settings)
{
	settings->setValue(this->name, this->value);
}

template<>
void Parameter<RGB>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    if(!value.isNull())
        this->value = RGB(value.value<QString>().toStdString());
}

template<>
void Parameter<RGB>::save(QSettings * settings)
{
	settings->setValue(this->name, value.hex().c_str());
}

template<>
void Parameter<InterpolationFilter>::load(QSettings * settings)
{
	QVariant value = settings->value(name, QVariant::fromValue(this->value));
	QString strValue = value.value<QString>();
	if(!value.isNull()) {
		if(strValue == "Internal")
			this->value = InterpolationFilter::Internal;
		else if(strValue == "NoInterpolation")
			this->value = InterpolationFilter::NoInterpolation;
		else if(strValue == "CubicInterpolation")
			this->value = InterpolationFilter::CubicInterpolation;
		else if(strValue == "LinearInterpolation")
			this->value = InterpolationFilter::LinearInterpolation;
		else if(strValue == "WindowedSincWith8Taps")
			this->value = InterpolationFilter::WindowedSincWith8Taps;
	}
}

template<>
void Parameter<InterpolationFilter>::save(QSettings * settings)
{
	switch(this->value) {
		case InterpolationFilter::Internal:
			settings->setValue(name, "Internal");
			break;
		case InterpolationFilter::NoInterpolation:
			settings->setValue(name, "NoInterpolation");
			break;
		case InterpolationFilter::CubicInterpolation:
			settings->setValue(name, "CubicInterpolation");
			break;
		case InterpolationFilter::LinearInterpolation:
			settings->setValue(name, "LinearInterpolation");
			break;
		case InterpolationFilter::WindowedSincWith8Taps:
			settings->setValue(name, "WindowedSincWith8Taps");
			break;
	}
}

template<>
void Parameter<AmigaFilterType>::load(QSettings * settings)
{
    QVariant value = settings->value(name, QVariant::fromValue(this->value));
    QString strValue = value.value<QString>();
    if(!value.isNull()) {
        if(strValue == "Auto")
            this->value = AmigaFilterType::Auto;
        else if(strValue == "Amiga500")
            this->value = AmigaFilterType::Amiga500;
        else if(strValue == "Amiga1200")
            this->value = AmigaFilterType::Amiga1200;
        else if(strValue == "Unfiltered")
            this->value = AmigaFilterType::Unfiltered;
        else if(strValue == "DisablePaulaEmulation")
            this->value = AmigaFilterType::DisablePaulaEmulation;
    }
}

template<>
void Parameter<AmigaFilterType>::save(QSettings * settings)
{
    switch(this->value) {
    case AmigaFilterType::Auto:
        settings->setValue(name, "Auto");
        break;
    case AmigaFilterType::Amiga500:
        settings->setValue(name, "Amiga500");
        break;
    case AmigaFilterType::Amiga1200:
        settings->setValue(name, "Amiga1200");
        break;
    case AmigaFilterType::Unfiltered:
        settings->setValue(name, "Unfiltered");
        break;
    case AmigaFilterType::DisablePaulaEmulation:
        settings->setValue(name, "DisablePaulaEmulation");
        break;
    }
}

template<>
void Parameter<BarType>::load(QSettings * settings)
{
    QVariant value = settings->value(name, QVariant::fromValue(this->value));
    QString strValue = value.value<QString>();
    if(!value.isNull()) {
        if(strValue == "Discrete")
            this->value = BarType::Discrete;
        else if(strValue == "Continuous")
            this->value = BarType::Continuous;
    }
}

template<>
void Parameter<BarType>::save(QSettings * settings)
{
    switch(this->value) {
    case BarType::Discrete:
        settings->setValue(name, "Discrete");
        break;
    case BarType::Continuous:
        settings->setValue(name, "Continuous");
        break;
    }
}

ParameterBase::ParameterBase(){}

ParameterBase::~ParameterBase(){}
