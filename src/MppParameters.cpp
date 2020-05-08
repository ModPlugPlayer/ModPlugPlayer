#include "MppParameters.hpp"

MppParameters::MppParameters()
{

}

void MppParameters::clearChangedFlags(){
    anyParameterChanged = false;
    repeatCountChanged = false;
    interpolationFilterChanged = false;
}

void MppParameters::update(MppParameters & mppParameters){
    if(mppParameters.isAnyParameterChanged()){
        if(this->getRepeatCount() != mppParameters.getRepeatCount())
            this->setRepeatCount(mppParameters.getRepeatCount());
        if(this->getInterpolationFilter() != mppParameters.getInterpolationFilter())
            this->setInterpolationFilter(mppParameters.getInterpolationFilter());
        if(this->getTimeUpdateFrequency() != mppParameters.getTimeUpdateFrequency())
            this->setTimeUpdateFrequency(mppParameters.getTimeUpdateFrequency());
    }
}


bool MppParameters::isAnyParameterChanged(){
    return anyParameterChanged;
}

bool MppParameters::isInterpolationFilterChanged(){
    return interpolationFilterChanged;
}

INTERPOLATIONFILTER MppParameters::getInterpolationFilter(){
    return interpolationFilter;
}

void MppParameters::setInterpolationFilter(INTERPOLATIONFILTER interpolationFilter){
    anyParameterChanged = true;
    interpolationFilterChanged = true;
    this->interpolationFilter = interpolationFilter;
}

bool MppParameters::isRepeatCountChanged(){
    return repeatCountChanged;
}

std::int32_t MppParameters::getRepeatCount(){
    return repeatCount;
}

void MppParameters::setRepeatCount(std::int32_t repeatCount){
    anyParameterChanged = true;
    repeatCountChanged = true;
    this->repeatCount = repeatCount;
}

bool MppParameters::isTimeUpdateFrequencyChanged(){
    return timeUpdateFrequencyChanged;
}

int MppParameters::getTimeUpdateFrequency(){
    return timeUpdateFrequency;
}

void MppParameters::setTimeUpdateFrequency(int timeUpdateFrequency){
    anyParameterChanged = true;
    timeUpdateFrequencyChanged = true;
    this->timeUpdateFrequency = timeUpdateFrequency;
}
