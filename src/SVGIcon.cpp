#include "SVGIcon.hpp"

SVGIcon::SVGIcon(){

}

SVGIcon::SVGIcon(const QString &svgData, const QString &svgColor){
	this->svgData = svgData;
	this->svgColor = svgColor;
}

void SVGIcon::setActiveColor(const RGB &color){
	this->activeColor = color;
	svgDataActive = svgData.replace(svgColor, color.hex().c_str());
	SVGIconEngine *engine = engineActiveIcon;
	engineActiveIcon = new SVGIconEngine(svgDataActive);
	if(engine != nullptr)
		delete engine;
}

void SVGIcon::setInactiveColor(const RGB &color){
	this->inactiveColor = color;
	svgDataInactive = svgData.replace(svgColor, color.hex().c_str());
	SVGIconEngine *engine = engineInactiveIcon;
	engineInactiveIcon = new SVGIconEngine(svgDataInactive);
	if(engine != nullptr)
		delete engine;
}

RGB SVGIcon::getActiveColor(){
	return activeColor;
}

RGB SVGIcon::getInactiveColor(){
	return inactiveColor;
}

QIcon SVGIcon::getActiveIcon(){
	if(engineActiveIcon == nullptr)
		return QIcon();
	else
		return QIcon(engineActiveIcon);
}

QIcon SVGIcon::getInactiveIcon(){
	if(engineInactiveIcon == nullptr)
		return QIcon();
	else
		return QIcon(engineInactiveIcon);
}
