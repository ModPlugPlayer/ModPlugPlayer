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
	void setColor(const RGB &color);
	RGB getColor();
    ~TitleBar();

private:
    Ui::TitleBar *ui;
};

#endif // TITLEBAR_HPP
