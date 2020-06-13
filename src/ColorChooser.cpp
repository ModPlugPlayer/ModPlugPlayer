#include "ColorChooser.hpp"
#include <QColorDialog>

ColorChooser::ColorChooser(QWidget *parent) : QPushButton(parent)
{
	connect(this,     SIGNAL(clicked()), this, SLOT(onColorChooserClicked()));

}

void ColorChooser::setColor(const RGB &color)
{
	this->color.setRgb(color.red, color.green, color.blue);
}

void ColorChooser::setColor(const QColor &color)
{
	this->color = color;
}

RGB ColorChooser::getRGBColor()
{
	RGB rgb;
	color.getRgb(&rgb.red, &rgb.green, &rgb.blue);
	return rgb;
}

QColor ColorChooser::getColor()
{
	return color;
}

void ColorChooser::onColorChooserClicked()
{
	color = QColorDialog::getColor(color, this);
	RGB rgb;
	color.getRgb(&rgb.red, &rgb.green, &rgb.blue);
	//QString fgColor = Y <= 170 ? "white" : "black";
	QString fgColor = DSP<double>::calculateBWForegroundColor(rgb, 10) ? "white" : "black";
	setStyleSheet(QString("QPushButton {background-color:rgb(%1,%2,%3);\ncolor:%4}").arg(rgb.red).arg(rgb.green).arg(rgb.blue).arg(fgColor));
	emit colorChanged();
}
