#ifndef SVGICON_HPP
#define SVGICON_HPP
#include <QString>
#include <SVGIconEngine.hpp>
#include <RGB.hpp>
#include <QIcon>

class SVGIcon
{
	public:
		SVGIcon();
		SVGIcon(const QString &svgData, const QString &svgColor);
		void setActiveColor(const RGB &color);
		void setInactiveColor(const RGB &color);
		RGB getActiveColor();
		RGB getInactiveColor();
		QIcon getActiveIcon();
		QIcon getInactiveIcon();
	private:
		QString svgData, svgDataActive, svgDataInactive, svgColor;
		RGB activeColor, inactiveColor;
		SVGIconEngine *engineInactiveIcon = nullptr, *engineActiveIcon = nullptr;
};

#endif // SVGICON_HPP
