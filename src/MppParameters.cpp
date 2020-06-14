#include "MppParameters.hpp"

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
	qRegisterMetaTypeStreamOperators<RGB>("RGB");

	this->settings = settings;

	addParameter(interpolationFilter, "Interpolation Filter");
	addParameter(volume, "Volume");
	addParameter(repeatCount, "Repeat Count");
	addParameter(timeUpdateFrequency, "Time Update Frequency");
	addParameter(spectrumAnalyzerBarAmount, "Spectrum Analyzer Bar Amount");
	addParameter(alwaysOnTop, "Always on Top");
	addParameter(titlebarTextColor, "TitleBar Text Color");
}

void MppParameters::save()
{
	for(ParameterBase *parameter:parameters) {
		parameter->save(settings);
	}
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
	if(!value.isNull())
		this->value = value.value<T>();
}

template<class T>
void Parameter<T>::save(QSettings * settings)
{
	QVariant value;
	value.setValue<T>(this->value);
	settings->setValue(this->name, value);
}

ParameterBase::ParameterBase()
{

}

ParameterBase::~ParameterBase()
{

}
