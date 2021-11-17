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

	addParameter(interpolationFilter, "InterpolationFilter");
	addParameter(volume, "Volume");
	addParameter(repeatCount, "RepeatCount");
	addParameter(timeUpdateFrequency, "TimeUpdateFrequency");
	addParameter(spectrumAnalyzerBarAmount, "SpectrumAnalyzerBarAmount");
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
	QVariant value = settings->value(name, QVariant::fromValue(this->value));
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

ParameterBase::ParameterBase(){}

ParameterBase::~ParameterBase(){}
