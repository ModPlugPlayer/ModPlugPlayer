/*
ColorChooser class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef COLORCHOOSER_HPP
#define COLORCHOOSER_HPP

#include <QPushButton>
#include <Util/ColorUtil.hpp>

class ColorChooser : public QPushButton
{
		Q_OBJECT
	public:
		explicit ColorChooser(QWidget *parent = nullptr);
		void setColor(const RGB &color);
		void setColor(const QColor &color);
		RGB getRGBColor();
		QColor getColor();
	signals:
		void colorChanged();
	private slots:
		void onColorChooserClicked();
	private:
		QColor color;

};

#endif // COLORCHOOSER_HPP
