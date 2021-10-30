#ifndef OPTIONBUTTON_HPP
#define OPTIONBUTTON_HPP

#include <QPushButton>
#include "SVGIcon.hpp"
#include <RGB.hpp>

class OptionButton : public QPushButton
{
		Q_OBJECT
	public:
        OptionButton(QWidget* parent = 0);
        void setActiveButtonLightColor(const RGB &color);
        void setInactiveButtonLightColor(const RGB &color);
    private:
        RGB activeButtonLightColor, inactiveButtonLightColor;
        QColor active, inactive;
        int buttonLightSize;
        int buttonLightPaddingSize;
        SVGIcon *iconButtonLight;
        qreal buttonLightBorderSize;
    private slots:
        /// paint the widget
        virtual void paintEvent(QPaintEvent* event);
};

#endif // OPTIONBUTTON_HPP
