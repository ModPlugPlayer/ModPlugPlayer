/*
ColorChooser class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "ColorChooser.hpp"
#include <QColorDialog>
#include <QDebug>

ColorChooser::ColorChooser(QWidget *parent) : QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(onColorChooserClicked()));
}

void ColorChooser::setColor(const RGB &color)
{
	QColor c(color.red, color.green, color.blue);
	setColor(c);
}

void ColorChooser::setColor(const QColor &color)
{
	this->color = color;
	int red, green, blue;
	color.getRgb(&red, &green, &blue);
	RGB rgb(red, green, blue);
	QString fgColor = DSP<double>::calculateBWForegroundColor(rgb, 10) ? "white" : "black";
	setStyleSheet(QString("QPushButton {background-color:rgb(%1,%2,%3);\ncolor:%4}").arg(rgb.red).arg(rgb.green).arg(rgb.blue).arg(fgColor));
	emit colorChanged();
}

RGB ColorChooser::getRGBColor()
{
	int red, green, blue;
	color.getRgb(&red, &green, &blue);
	RGB rgb(red, green, blue);
	return rgb;
}

QColor ColorChooser::getColor()
{
	return color;
}

void ColorChooser::onColorChooserClicked()
{
	QColor color = QColorDialog::getColor(this->color, this);

	if(color.isValid()) {
		setColor(color);
	}
}
