#include "MppParameters.hpp"

bool ParameterBase::isDirty(){
	return dirty;
}

void ParameterBase::resetDirtyState(){
	this->dirty = false;
}

std::string ParameterBase::getName() {
	return name;
}

MppParameters::MppParameters(QSettings *settings)
{
	this->settings = settings;
	addParameter(interpolationFilter, "Interpolation Filter");
	addParameter(volume, "Volume");
	addParameter(repeatCount, "Repeat Count");
	addParameter(timeUpdateFrequency, "Time Update Frequency");
	addParameter(spectrumAnalyzerBarAmount, "Spectrum Analyzer Bar Amount");
	addParameter(alwaysOnTop, "Always on Top");
}

void MppParameters::save()
{
	for(ParameterBase *parameter:parameters) {
		//parameter->save();
	}
}

void MppParameters::load()
{
	for(ParameterBase *parameter:parameters) {
		//parameter->load();
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

void MppParameters::addParameter(ParameterBase &parameter, std::string name)
{
	parameter.name = name;
	parameters.push_back(&parameter);
}

template<class T>
void Parameter<T>::load()
{

}

template<class T>
void Parameter<T>::save()
{

}

ParameterBase::ParameterBase()
{

}

ParameterBase::~ParameterBase()
{

}
