#ifndef TITLEBAR_HPP
#define TITLEBAR_HPP

#include <QWidget>
#include <RGB.hpp>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
	void setActiveColor(const RGB &color);
	RGB getActiveColor();
	void setInactiveColor(const RGB &color);
	RGB getInactiveColor();
	~TitleBar();

private:
    Ui::TitleBar *ui;
	RGB activeColor, inactiveColor;
	void setStyleSheetColor(RGB color);
};

#endif // TITLEBAR_HPP
