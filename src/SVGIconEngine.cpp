/*
SVGIconEngine class definitions of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "SVGIconEngine.hpp"
#include <QPainter>

SVGIconEngine::SVGIconEngine() {

}

SVGIconEngine::SVGIconEngine(const std::string &iconBuffer) {
  data = QByteArray::fromStdString(iconBuffer);
}

SVGIconEngine::SVGIconEngine(const QString &iconBuffer) {
  data = iconBuffer.toUtf8();
}

void SVGIconEngine::setSVGData(std::string &iconBuffer){
  data = QByteArray::fromStdString(iconBuffer);
}

void SVGIconEngine::setSVGData(QString &iconBuffer){
  data = iconBuffer.toUtf8();
}

void SVGIconEngine::paint(QPainter *painter, const QRect &rect,
						  QIcon::Mode mode, QIcon::State state) {
  QSvgRenderer renderer(data);
  renderer.setAspectRatioMode(Qt::AspectRatioMode::KeepAspectRatio);
  renderer.render(painter, rect);
}

QIconEngine *SVGIconEngine::clone() const { return new SVGIconEngine(*this); }

QPixmap SVGIconEngine::pixmap(const QSize &size, QIcon::Mode mode,
							  QIcon::State state) {
  // This function is necessary to create an EMPTY pixmap. It's called always
  // before paint()

  QImage img(size, QImage::Format_ARGB32);
  img.fill(qRgba(0, 0, 0, 0));
  QPixmap pix = QPixmap::fromImage(img, Qt::NoFormatConversion);
  {
	QPainter painter(&pix);
	QRect r(QPoint(0.0, 0.0), size);
	this->paint(&painter, r, mode, state);
  }
  return pix;
}
