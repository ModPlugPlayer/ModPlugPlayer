#ifndef OPTIONBUTTONS_HPP
#define OPTIONBUTTONS_HPP

#include <QWidget>

namespace Ui {
class OptionButtons;
}

class OptionButtons : public QWidget
{
    Q_OBJECT

public:
    explicit OptionButtons(QWidget *parent = nullptr);
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
