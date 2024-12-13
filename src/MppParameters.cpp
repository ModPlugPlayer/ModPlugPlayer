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

QGradientStops getDefaultGradientStops() {
    QGradientStops gradientStops;
    gradientStops.append(QPair<double,QColor>(1.0, Qt::red));
    gradientStops.append(QPair<double,QColor>(0.6, QColor(255, 210, 0)));
    gradientStops.append(QPair<double,QColor>(0.5, Qt::yellow));
    gradientStops.append(QPair<double,QColor>(0.25, QColor(175, 255, 0)));
    gradientStops.append(QPair<double,QColor>(0.0, QColor(0, 200, 0)));
    return gradientStops;
}

MppParameters::MppParameters(QSettings *settings)
{
    //qRegisterMetaTypeStreamOperators<RGB>("RGB");

	this->settings = settings;

    addParameter(audioDeviceIndex, "AudioDeviceIndex");
	addParameter(interpolationFilter, "InterpolationFilter");
    addParameter(amigaFilter, "AmigaFilterType");
    addParameter(volume, "Volume");
    addParameter(repeatMode, "RepeatMode");
    addParameter(eqEnabled, "EqEnabled");
    addParameter(dspEnabled, "DSPEnabled");
    addParameter(xBassEnabled, "XBassEnabled");
    addParameter(surroundEnabled, "SurroundEnabled");
    addParameter(reverbEnabled, "ReverbEnabled");
    addParameter(timeUpdateFrequency, "TimeUpdateFrequency");
    addParameter(playAutomaticallyAfterLoading, "PlayAutomaticallyAfterLoading");
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

    addParameter(spectrumAnalyzerType, "SpectrumAnalyzer/Type");
    addParameter(spectrumAnalyzerWindowFunction, "SpectrumAnalyzer/WindowFunction");
    addParameter(spectrumAnalyzerMaximumValue, "SpectrumAnalyzer/MaximumValue");
    addParameter(spectrumAnalyzerBarAmount, "SpectrumAnalyzer/BarAmount");
    addParameter(spectrumAnalyzerLedAmount, "SpectrumAnalyzer/LED-Amount");
    addParameter(spectrumAnalyzerLedHeightRatio, "SpectrumAnalyzer/LED-HeightRatio");
    addParameter(spectrumAnalyzerBarWidthRatio, "SpectrumAnalyzer/BarWidthRatio");
    addParameter(spectrumAnalyzerDimmedTransparencyRatio, "SpectrumAnalyzer/DimmedTransparencyRatio");
    addParameter(spectrumAnalyzerDimmingRatio, "SpectrumAnalyzer/DimmingRatio");
    addParameter(spectrumAnalyzerGradient, "SpectrumAnalyzer/Gradient");

    addParameter(vuMeterType, "VU-Meter/Type");
    addParameter(vuMeterMaximumValue, "VU-Meter/MaximumValue");
    addParameter(vuMeterMinimumValue, "VU-Meter/MinimumValue");
    addParameter(vuMeterLedAmount, "VU-Meter/LED-Amount");
    addParameter(vuMeterLedHeightRatio, "VU-Meter/LED-HeightRatio");
    addParameter(vuMeterDimmedTransparencyRatio, "VU-Meter/DimmedTransparencyRatio");
    addParameter(vuMeterDimmingRatio, "VU-Meter/DimmingRatio");
    addParameter(vuMeterGradient, "VU-Meter/Gradient");

    addParameter(playerWindowSize, "WindowSize/Player");
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
    QVariant value = settings->value(name);
    if(!value.isNull() && value.isValid())
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
    if(!value.isNull() && value.isValid())
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
    QVariant value = settings->value(name);
	QString strValue = value.value<QString>();
    if(!value.isNull() && value.isValid()) {
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
void Parameter<AmigaFilter>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    QString strValue = value.value<QString>();
    if(!value.isNull() && value.isValid()) {
        if(strValue == "Auto")
            this->value = AmigaFilter::Auto;
        else if(strValue == "Amiga500")
            this->value = AmigaFilter::Amiga500;
        else if(strValue == "Amiga1200")
            this->value = AmigaFilter::Amiga1200;
        else if(strValue == "Unfiltered")
            this->value = AmigaFilter::Unfiltered;
        else if(strValue == "DisablePaulaEmulation")
            this->value = AmigaFilter::DisablePaulaEmulation;
    }
}

template<>
void Parameter<AmigaFilter>::save(QSettings * settings)
{
    switch(this->value) {
    case AmigaFilter::Auto:
        settings->setValue(name, "Auto");
        break;
    case AmigaFilter::Amiga500:
        settings->setValue(name, "Amiga500");
        break;
    case AmigaFilter::Amiga1200:
        settings->setValue(name, "Amiga1200");
        break;
    case AmigaFilter::Unfiltered:
        settings->setValue(name, "Unfiltered");
        break;
    case AmigaFilter::DisablePaulaEmulation:
        settings->setValue(name, "DisablePaulaEmulation");
        break;
    }
}

template<>
void Parameter<RepeatMode>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    QString strValue = value.value<QString>();
    if(!value.isNull() && value.isValid()) {
        if(strValue == "NoRepeat")
            this->value = RepeatMode::NoRepeat;
        else if(strValue == "RepeatTrack")
            this->value = RepeatMode::RepeatTrack;
        else if(strValue == "LoopTrack")
            this->value = RepeatMode::LoopTrack;
        else if(strValue == "RepeatPlayList")
            this->value = RepeatMode::RepeatPlayList;
    }
}

template<>
void Parameter<RepeatMode>::save(QSettings * settings)
{
    switch(this->value) {
    case RepeatMode::NoRepeat:
        settings->setValue(name, "NoRepeat");
        break;
    case RepeatMode::RepeatTrack:
        settings->setValue(name, "RepeatTrack");
        break;
    case RepeatMode::LoopTrack:
        settings->setValue(name, "LoopTrack");
        break;
    case RepeatMode::RepeatPlayList:
        settings->setValue(name, "RepeatPlayList");
        break;
    }
}
template<>
void Parameter<BarType>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    QString strValue = value.value<QString>();
    if(!value.isNull() && value.isValid()) {
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

template<>
void Parameter<WindowFunction>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    if(!value.isNull() && value.isValid()) {
        QString strValue = value.value<QString>();
        if(strValue == "None")
            this->value = WindowFunction::None;
        else if(strValue == "HanningWindow")
            this->value = WindowFunction::HanningWindow;
        else if(strValue == "HammingWindow")
            this->value = WindowFunction::HammingWindow;
        else if(strValue == "BlackmanWindow")
            this->value = WindowFunction::BlackmanWindow;
    }
}

template<>
void Parameter<WindowFunction>::save(QSettings * settings)
{
    switch(this->value) {
    case WindowFunction::None:
        settings->setValue(name, "None");
        break;
    case WindowFunction::HanningWindow:
        settings->setValue(name, "HanningWindow");
        break;
    case WindowFunction::HammingWindow:
        settings->setValue(name, "HammingWindow");
        break;
    case WindowFunction::BlackmanWindow:
        settings->setValue(name, "BlackmanWindow");
        break;
    }
}

QList<QString> serializeQGradientStops(const QGradientStops & gradientStops)
{
    QList<QString> output;
    for(const QGradientStop &gradientStop : gradientStops) {
        output += QString::number(gradientStop.first) + " " + gradientStop.second.name(QColor::NameFormat::HexRgb);
    }
    return output;
}

QGradientStops deSerializeQGradientStops(const QList<QString> & input)
{
    QGradientStops gradientStops;

    for(const QString &point : input) {
        QStringList tokens = point.split(" ");
        if(tokens.length() != 2)
            continue;
        QGradientStop gradientStop;
        bool ok;
        gradientStop.first = tokens[0].toDouble(&ok);
        if(!ok)
            continue;
        gradientStop.second = QColor(tokens[1]);
        gradientStops.append(gradientStop);
    }
    return gradientStops;
}


template<>
void Parameter<QGradientStops>::load(QSettings * settings)
{
    QVariant value = settings->value(name);
    if(!value.isNull() && value.isValid()) {
        QGradientStops gradientStops = deSerializeQGradientStops(value.toStringList());
        this->value = gradientStops;
    }
}

template<>
void Parameter<QGradientStops>::save(QSettings * settings)
{
    settings->setValue(name, serializeQGradientStops(this->value));
}

ParameterBase::ParameterBase(){}

ParameterBase::~ParameterBase(){}
