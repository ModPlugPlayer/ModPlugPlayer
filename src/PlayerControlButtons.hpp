#ifndef PLAYERCONTROLBUTTONS_HPP
#define PLAYERCONTROLBUTTONS_HPP

#include <QWidget>
#include <QFileDialog>
#include <RGB.hpp>
#include "Enums.hpp"
#include "SVGIcon.hpp"

namespace Ui {
class PlayerControlButtons;
}

class PlayerControlButtons : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControlButtons(QWidget *parent = nullptr);
    ~PlayerControlButtons();
	void setActiveButtonLightColor(const RGB &color);
	void setInactiveButtonLightColor(const RGB &color);
	void setState(const PlayerState &state);
	void refresh();
signals:
    void open();
    void play();
    void pause();
    void stop();
    void setup();
    void rewind();
    void fastForward();
    void previous();
    void next();

public slots:
	void on_playerState_changed(PlayerState playerState);

private slots:

	void on_openButton_pressed();

	void on_openButton_released();

	void on_rewindButton_pressed();

	void on_rewindButton_released();

	void on_fastForwardButton_pressed();

	void on_fastForwardButton_released();

	void on_previousButton_pressed();

	void on_previousButton_released();

	void on_nextButton_pressed();

	void on_nextButton_released();

	private:

	Ui::PlayerControlButtons *ui;

	QString getResourceContent(std::string resource);

	PlayerState state;

	RGB activeButtonLightColor, inactiveButtonLightColor;

	SVGIcon *iconOpen, *iconPlay, *iconPause, *iconStop,
			*iconRewind, *iconFastForward, *iconPrevious, *iconNext;
};

#endif // PLAYERCONTROLBUTTONS_HPP
