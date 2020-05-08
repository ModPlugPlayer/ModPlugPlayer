#ifndef PLAYERCONTROLBUTTONS_HPP
#define PLAYERCONTROLBUTTONS_HPP

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class PlayerControlButtons;
}

class PlayerControlButtons : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControlButtons(QWidget *parent = nullptr);
    ~PlayerControlButtons();

signals:
    void open(QString filePath);
    void play();
    void pause();
    void stop();
    void setup();
    void rewind();
    void fastForward();
    void previous();
    void next();

private slots:

    void on_openButton_clicked();

private:
    Ui::PlayerControlButtons *ui;
    QFileDialog *fileDialog;
};

#endif // PLAYERCONTROLBUTTONS_HPP
