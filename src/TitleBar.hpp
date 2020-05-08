#ifndef TITLEBAR_HPP
#define TITLEBAR_HPP

#include <QWidget>

namespace Ui {
class TitleBar;
}

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

private:
    Ui::TitleBar *ui;
};

#endif // TITLEBAR_HPP
