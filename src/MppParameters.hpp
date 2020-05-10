#ifndef MPPPARAMETERS_HPP
#define MPPPARAMETERS_HPP
#include "Enums.hpp"
#include <cstdint>
#include <algorithm>

template<class T>
class Parameter {
private:
    T value;
    bool dirty;
public:
    Parameter(const T& value) {
        this->value = value;
        dirty = false;
    }
    T operator=(const T& value) {
        this->value = value;
        dirty = true;
        return value;
    }

    //implicit conversion
    operator T() const {
        return value;
    }

    bool isDirty(){
        return dirty;
    }

    void resetDirtyState(){
        this->dirty = false;
    }
};

class MppParameters
{
public:
    MppParameters();
    void clearChangedFlags();
    bool isAnyParameterChanged();
    void update(MppParameters & mppParameters);

    bool isInterpolationFilterChanged();
    INTERPOLATIONFILTER getInterpolationFilter();
    void setInterpolationFilter(INTERPOLATIONFILTER interpolationFilter);

    bool isRepeatCountChanged();
    std::int32_t getRepeatCount();
    void setRepeatCount(std::int32_t repeatCount);

    bool isTimeUpdateFrequencyChanged();
    size_t getTimeUpdateFrequency();
    void setTimeUpdateFrequency(size_t timeUpdateFrequency);

    bool isBarAmountChanged();
    size_t getBarAmount();
    void setBarAmount(size_t timeUpdateFrequency);

private:
    bool anyParameterChanged = false;
    Parameter<INTERPOLATIONFILTER> interpolationFilter = INTERPOLATIONFILTER::INTERNAL;
    Parameter<size_t> repeatCount = 0;
    Parameter<size_t> timeUpdateFrequency = 4;
    Parameter<size_t> barAmount = 20;
};

#endif // MPPPARAMETERS_HPP
