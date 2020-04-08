#ifndef MPPPARAMETERS_HPP
#define MPPPARAMETERS_HPP
#include "Enums.hpp"
#include <cstdint>

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
    int getTimeUpdateFrequency();
    void setTimeUpdateFrequency(int timeUpdateFrequency);

private:
    bool anyParameterChanged = false;
    bool interpolationFilterChanged = false;
    bool repeatCountChanged = false;
    bool timeUpdateFrequencyChanged = false;
    INTERPOLATIONFILTER interpolationFilter = INTERPOLATIONFILTER::INTERNAL;
    std::int32_t repeatCount = 0;
    int timeUpdateFrequency = 4;
};

#endif // MPPPARAMETERS_HPP
