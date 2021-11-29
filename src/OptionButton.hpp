#ifndef OPTIONBUTTON_HPP
#define OPTIONBUTTON_HPP

#include <QPushButton>
#include "SVGIcon.hpp"
#include <RGB.hpp>
#include "LedButton.hpp"

class OptionButton : public QPushButton, public LedButton
{
		Q_OBJECT
	public:
        OptionButton(QWidget* parent = 0);
        void setActiveButtonLightColor(const RGB &color) override;
        void setInactiveButtonLightColor(const RGB &color) override;
        void setBackgroundColor(const RGB &color) override;
        void setTextColor(const RGB &color) override;
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
    protected:
        void refreshStyleSheet() override;
};

#endif // OPTIONBUTTON_HPP
