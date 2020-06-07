#ifndef SVGICONENGINE_HPP
#define SVGICONENGINE_HPP

#include <QIconEngine>
#include <QSvgRenderer>

class SVGIconEngine : public QIconEngine {

  QByteArray data;

public:
  explicit SVGIconEngine(const std::string &iconBuffer);
  void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode,
			 QIcon::State state) override;
  QIconEngine *clone() const override;
  QPixmap pixmap(const QSize &size, QIcon::Mode mode,
				 QIcon::State state) override;
signals:

public slots:
};

#endif // SVGICONENGINE_HPP
