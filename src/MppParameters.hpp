#ifndef MPPPARAMETERS_HPP
#define MPPPARAMETERS_HPP
#include "Enums.hpp"
#include <cstdint>
#include <algorithm>
#include <QString>
#include <vector>
#include <QSettings>
#include <DSP.hpp>
#include <QDataStream>
#include <QMetaType>
#ifdef QT_VERSION
#endif

class ParameterBase {
	public:
		ParameterBase();
		~ParameterBase();

		bool isDirty();

		void resetDirtyState();

		QString getName();

		virtual void save(QSettings * settings) = 0;
		virtual void load(QSettings * settings) = 0;

	protected:
		friend class MppParameters;
		bool dirty = false;
		QString name;
};

template<class T>
class Parameter : public ParameterBase{
private:
	friend class MppParameters;
	T value;
public:
	Parameter(const T& value);

	T operator=(const T& value);
	//implicit conversion
	operator T() const;

	void load(QSettings * settings) override;
	void save(QSettings * settings) override;
};

class MppParameters
{
public:
	MppParameters(QSettings *settings);
	void save();
	void load();
    void clearChangedFlags();
    bool isAnyParameterChanged();

	Parameter<InterpolationFilter> interpolationFilter = InterpolationFilter::Internal;
	Parameter<int> volume = 0;
	Parameter<int> repeatCount = 0;
	Parameter<int> timeUpdateFrequency = 4;
	Parameter<int> spectrumAnalyzerBarAmount = 0;
	Parameter<bool> alwaysOnTop = false;
	Parameter<bool> hideTitleBar = false;
	Parameter<bool> hideByCloseButton = false;
	Parameter<bool> enableRightClickMenu = false;
	Parameter<bool> enableSystemTray = false;
	Parameter<bool> minimizeToSystemTray = false;
	Parameter<bool> hideApplicationIcon = false;
	Parameter<RGB> activeTitlebarTextColor = RGB(0,0,0);
	Parameter<RGB> inactiveTitlebarTextColor = RGB(255,255,255);
	Parameter<RGB> activeButtonLightColor = RGB(7,255,52);
	Parameter<RGB> inactiveButtonLightColor = RGB(7,128,52);
	Parameter<RGB> playerBodyTextColor = RGB(0,0,0);
	Parameter<RGB> playerBodyBackgroundColor = RGB(192,192,192);
	Parameter<RGB> lcdDisplayForegroundColor = RGB(0,0,0);
	Parameter<RGB> lcdDisplayBackgroundColor = RGB(188,228,197);
    Parameter<int> spectrumAnalyzerLedAmount = 14;
    Parameter<int> vuMeterLedAmount = 14;

private:
	std::vector<ParameterBase *> parameters;
	void addParameter(ParameterBase &parameter, QString name);
	QSettings * settings;
};

////
template<class T>
Parameter<T>::Parameter(const T& value) {
	this->value = value;
	dirty = false;
}

template<class T>
T Parameter<T>::operator=(const T &value) {
	if(this->value != value) {
		this->value = value;
		dirty = true;
	}
	return this->value;
}

//implicit conversion
template<class T>
Parameter<T>::operator T() const {
	return value;
}

#endif // MPPPARAMETERS_HPP
