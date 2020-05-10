#include "MppParameters.hpp"

MppParameters::MppParameters()
{

}

void MppParameters::clearChangedFlags(){
    anyParameterChanged = false;
    repeatCount.resetDirtyState();
    interpolationFilter.resetDirtyState();
    timeUpdateFrequency.resetDirtyState();
    barAmount.resetDirtyState();
}

void MppParameters::update(MppParameters & mppParameters){
    if(mppParameters.isAnyParameterChanged()){
        if(this->getRepeatCount() != mppParameters.getRepeatCount())
            this->setRepeatCount(mppParameters.getRepeatCount());
        if(this->getInterpolationFilter() != mppParameters.getInterpolationFilter())
            this->setInterpolationFilter(mppParameters.getInterpolationFilter());
        if(this->getTimeUpdateFrequency() != mppParameters.getTimeUpdateFrequency())
            this->setTimeUpdateFrequency(mppParameters.getTimeUpdateFrequency());
        if(this->getBarAmount() != mppParameters.getBarAmount())
            this->setBarAmount(mppParameters.getBarAmount());
    }
}


bool MppParameters::isAnyParameterChanged(){
    return anyParameterChanged;
}

bool MppParameters::isInterpolationFilterChanged(){
    return interpolationFilter.isDirty();
}

INTERPOLATIONFILTER MppParameters::getInterpolationFilter(){
    return interpolationFilter;
}

void MppParameters::setInterpolationFilter(INTERPOLATIONFILTER interpolationFilter){
    anyParameterChanged = true;
    this->interpolationFilter = interpolationFilter;
}

bool MppParameters::isRepeatCountChanged(){
    return repeatCount.isDirty();
}

std::int32_t MppParameters::getRepeatCount(){
    return repeatCount;
}

void MppParameters::setRepeatCount(std::int32_t repeatCount){
    anyParameterChanged = true;
    this->repeatCount = repeatCount;
}

bool MppParameters::isTimeUpdateFrequencyChanged(){
    return timeUpdateFrequency.isDirty();
}

size_t MppParameters::getTimeUpdateFrequency(){
    return timeUpdateFrequency;
}

void MppParameters::setTimeUpdateFrequency(size_t timeUpdateFrequency){
    anyParameterChanged = true;
    this->timeUpdateFrequency = timeUpdateFrequency;
}

bool MppParameters::isBarAmountChanged(){
    return barAmount.isDirty();
}

size_t MppParameters::getBarAmount(){
    return barAmount;
}

void MppParameters::setBarAmount(size_t barAmount){
    anyParameterChanged = true;
    this->barAmount = barAmount;
}
