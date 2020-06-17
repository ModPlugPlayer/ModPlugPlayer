#include "SVGIcon.hpp"

SVGIcon::SVGIcon(){

}

SVGIcon::SVGIcon(const QString &svgData, const QString &svgColor){
	this->svgData = svgData;
	this->svgColor = svgColor;
}

void SVGIcon::setActiveColor(const RGB &color){
	this->activeColor = color;
	svgDataActive = svgColor;
	svgDataActive = svgDataActive.replace(svgColor, color.hex().c_str());
	SVGIconEngine *engine = engineActiveIcon;
	engineActiveIcon = new SVGIconEngine(svgDataActive);
	activeIcon = QIcon(engineActiveIcon);
	if(engine != nullptr)
		delete engine;
}

void SVGIcon::setInactiveColor(const RGB &color){
	this->inactiveColor = color;
	svgDataInactive = svgData;
	svgDataInactive = svgDataInactive.replace(svgColor, color.hex().c_str());
	SVGIconEngine *engine = engineInactiveIcon;
	engineInactiveIcon = new SVGIconEngine(svgDataInactive);
	inactiveIcon = QIcon(engineInactiveIcon);
	if(engine != nullptr)
		delete engine;
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
