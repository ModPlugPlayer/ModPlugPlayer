#include "LedButton.hpp"

void LedButton::setActiveButtonLightColor(const RGB & color) {
    activeButtonLightColor = color;
}

void LedButton::setInactiveButtonLightColor(const RGB & color) {
    inactiveButtonLightColor = color;
}

RGB LedButton::getActiveButtonLightColor() const {
    return activeButtonLightColor;
}

RGB LedButton::getInactiveButtonLightColor() const {
    return inactiveButtonLightColor;
}
