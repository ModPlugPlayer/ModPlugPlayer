#include "SVGIcon.hpp"

SVGIcon::SVGIcon(const QString &svgData, const QString &svgColor){
	this->svgData = svgData;
	this->svgColor = svgColor;
	activeIcon = QIcon(&engineActiveIcon);
	inactiveIcon = QIcon(&engineInactiveIcon);
}

void SVGIcon::setActiveColor(const RGB &color){
	this->activeColor = color;
	svgDataActive = svgData;
	svgDataActive = svgDataActive.replace(svgColor, color.hex().c_str());
	engineActiveIcon.setSVGData(svgDataActive);
	//activeIcon = QIcon(&engineActiveIcon);
}

void SVGIcon::setInactiveColor(const RGB &color){
	this->inactiveColor = color;
	svgDataInactive = svgData;
	svgDataInactive = svgDataInactive.replace(svgColor, color.hex().c_str());
	engineInactiveIcon.setSVGData(svgDataInactive);
	//inactiveIcon = QIcon(&engineInactiveIcon);
}

RGB SVGIcon::getActiveColor(){
	return activeColor;
}

RGB SVGIcon::getInactiveColor(){
	return inactiveColor;
}

QIcon & SVGIcon::getActiveIcon(){
	return activeIcon;
}

QIcon & SVGIcon::getInactiveIcon(){
	return inactiveIcon;
}
