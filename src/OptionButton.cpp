#include "OptionButton.hpp"
#include <QPainter>
#include "SVGIconEngine.hpp"
#include "ResourceUtil.hpp"

OptionButton::OptionButton(QWidget* parent):QPushButton(parent)
{
    setActiveButtonLightColor(RGB(std::string("#00FF00")));
    setInactiveButtonLightColor(RGB(std::string("#00FFFF")));
    buttonLightSize = 8;
    buttonLightPaddingSize = 3;
    buttonLightBorderSize = 0.4;
    iconButtonLight = new SVGIcon(ResourceUtil::getResourceContent(":/Graphics/Vectoral/toprightcorner.svg"), "#00ff00");
}

void OptionButton::setActiveButtonLightColor(const RGB & color)
{
    LedButton::setActiveButtonLightColor(color);
    active = QColor(color.red, color.green, color.blue);
    //iconButtonLight->setActiveColor(color);

}

void OptionButton::setInactiveButtonLightColor(const RGB & color)
{
    LedButton::setInactiveButtonLightColor(color);
    inactive = QColor(color.red, color.green, color.blue);
    //iconButtonLight->setInactiveColor(color);
}

void OptionButton::setBackgroundColor(const RGB & color)
{
    LedButton::setBackgroundColor(color);
}

void OptionButton::setTextColor(const RGB & color)
{
    LedButton::setTextColor(color);
}

void OptionButton::paintEvent(QPaintEvent * event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::VerticalSubpixelPositioning);
    painter.setPen(QPen(Qt::black, buttonLightBorderSize));
    int shiftingAmount;
    if(QPushButton::isDown()) {
        shiftingAmount = 1;
        painter.setBrush(active);
    }
    else {
        shiftingAmount = 0;
        painter.setBrush(inactive);
    }
    QPolygon pp;
    pp << QPoint(width() - buttonLightSize - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) << QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightPaddingSize + shiftingAmount) <<QPoint(width() - buttonLightPaddingSize - shiftingAmount, buttonLightSize + buttonLightPaddingSize + shiftingAmount);
    painter.drawPolygon(pp, Qt::OddEvenFill);
    painter.restore();
}

void OptionButton::refreshStyleSheet() {

}
