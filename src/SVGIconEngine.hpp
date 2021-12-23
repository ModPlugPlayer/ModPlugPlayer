/*
SVGIconEngine class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SVGICONENGINE_HPP
#define SVGICONENGINE_HPP

#include <QIconEngine>
#include <QSvgRenderer>

class SVGIconEngine : public QIconEngine {

  QByteArray data;

public:
  explicit SVGIconEngine();
  explicit SVGIconEngine(const std::string &iconBuffer);
  explicit SVGIconEngine(const QString &iconBuffer);
  void setSVGData(std::string &iconBuffer);
  void setSVGData(QString &iconBuffer);
  void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode,
			 QIcon::State state) override;
  QIconEngine *clone() const override;
  QPixmap pixmap(const QSize &size, QIcon::Mode mode,
				 QIcon::State state) override;
signals:

public slots:
};

#endif // SVGICONENGINE_HPP
