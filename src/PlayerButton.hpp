#pragma once
#include <RGB.hpp>

class PlayerButton {
public:
    virtual void setBackgroundColor(const RGB &color);
    virtual void setTextColor(const RGB &color);
    RGB getBackgroundColor() const;
    RGB getTextColor() const;
    const QString & getStyleSheetTemplate() const;
    void setStyleSheetTemplate(const QString & styleSheetTemplate);

protected:
    RGB backgroundColor;
    RGB textColor;
    QString styleSheetTemplate;
    virtual void refreshStyleSheet() = 0;
};
