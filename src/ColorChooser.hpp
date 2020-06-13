#ifndef COLORCHOOSER_HPP
#define COLORCHOOSER_HPP

#include <QPushButton>
#include <DSP.hpp>

class ColorChooser : public QPushButton
{
		Q_OBJECT
	public:
		explicit ColorChooser(QWidget *parent = nullptr);
		void setColor(const RGB<unsigned char> &color);
		void setColor(const QColor &color);
		RGB<unsigned char> getRGBColor();
		QColor getColor();
	signals:
		void colorChanged();
	private slots:
		void onColorChooserClicked();
	private:
		QColor color;

};

#endif // COLORCHOOSER_HPP
