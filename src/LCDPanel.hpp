#ifndef LCDPANEL_HPP
#define LCDPANEL_HPP

#include <QWidget>
#include "PlayerButton.hpp"

namespace Ui {
class LCDPanel;
}

class LCDPanel : public QWidget, public PlayerButton
{
    Q_OBJECT

public:
    explicit LCDPanel(QWidget *parent = nullptr);
    void updateTime(int seconds);
    void setSongTitle(QString songTitle);
    void setSongDuration(size_t songDurationSeconds);
    void setBackgroundColor(const RGB &color) override;
    void setTextColor(const RGB &color) override;
    ~LCDPanel();

private:
    Ui::LCDPanel *ui;
    QFont *SevenSegment;

protected:
    void refreshStyleSheet() override;
};

#endif // LCDPANEL_HPP
