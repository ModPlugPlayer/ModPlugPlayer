#ifndef OPTIONBUTTONS_HPP
#define OPTIONBUTTONS_HPP

#include <QWidget>
#include <RGB.hpp>

namespace Ui {
class OptionButtons;
}

class OptionButtons : public QWidget
{
    Q_OBJECT

public:
    explicit OptionButtons(QWidget *parent = nullptr);
    void setActiveButtonLightColor(const RGB &color);
    void setInactiveButtonLightColor(const RGB &color);
    ~OptionButtons();
signals:
    void info();
    void repeat();
    void saveAs();
    void del();
    void details();
    void shuffle();
    void saveList();
    void clear();
    void about();
    void playlist();
    void saveWav();
    void remove();
private:
    Ui::OptionButtons *ui;
};

#endif // OPTIONBUTTONS_HPP
