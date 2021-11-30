#pragma once
#include "PlayerButton.hpp"

class LedButton : public PlayerButton {
public:
    virtual void setActiveButtonLightColor(const RGB &color);
    virtual void setInactiveButtonLightColor(const RGB &color);
    RGB getActiveButtonLightColor() const;
    RGB getInactiveButtonLightColor() const;
protected:
    RGB activeButtonLightColor;
    RGB inactiveButtonLightColor;
};
