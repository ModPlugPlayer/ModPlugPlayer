#ifndef OPTIONBUTTONS_HPP
#define OPTIONBUTTONS_HPP

#include <QWidget>
#include <RGB.hpp>
#include "LedButton.hpp"
#include <vector>

namespace Ui {
class OptionButtons;
}

class OptionButtons : public QWidget, public LedButton
{
    Q_OBJECT

public:
    explicit OptionButtons(QWidget *parent = nullptr);
    void setActiveButtonLightColor(const RGB &color) override;
    void setInactiveButtonLightColor(const RGB &color) override;
    void setBackgroundColor(const RGB &color) override;
    void setTextColor(const RGB &color) override;
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
protected:
    void refreshStyleSheet() override;
    std::vector<LedButton *> buttons;
};

#endif // OPTIONBUTTONS_HPP
