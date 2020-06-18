#ifndef SVGICON_HPP
#define SVGICON_HPP
#include <QString>
#include <SVGIconEngine.hpp>
#include <RGB.hpp>
#include <QIcon>
#include <QObject>

class SVGIcon : public QObject
{
	Q_OBJECT
	public:
		SVGIcon(const QString &svgData, const QString &svgColor);
		void setActiveColor(const RGB &color);
		void setInactiveColor(const RGB &color);
		RGB getActiveColor();
		RGB getInactiveColor();
		QIcon & getActiveIcon();
		QIcon & getInactiveIcon();
	private:
		QString svgData, svgDataActive, svgDataInactive, svgColor;
		RGB activeColor, inactiveColor;
		QIcon activeIcon, inactiveIcon;
		SVGIconEngine engineInactiveIcon, engineActiveIcon;
};

#endif // SVGICON_HPP
