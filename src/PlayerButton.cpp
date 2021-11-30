#include "PlayerButton.hpp"

void PlayerButton::setBackgroundColor(const RGB & color) {
    backgroundColor = color;
}

void PlayerButton::setTextColor(const RGB & color) {
    textColor = color;
}

RGB PlayerButton::getBackgroundColor() const {
    return backgroundColor;
}

RGB PlayerButton::getTextColor() const {
    return textColor;
}

const QString & PlayerButton::getStyleSheetTemplate() const {
    return styleSheetTemplate;
}

void PlayerButton::setStyleSheetTemplate(const QString & styleSheetTemplate) {
    this->styleSheetTemplate = styleSheetTemplate;
}
