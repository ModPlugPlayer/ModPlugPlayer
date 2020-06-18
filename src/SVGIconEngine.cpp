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
