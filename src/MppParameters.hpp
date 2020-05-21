#ifndef MPPPARAMETERS_HPP
#define MPPPARAMETERS_HPP
#include "Enums.hpp"
#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include <QSettings>

class ParameterBase {
	public:
		ParameterBase();
		~ParameterBase();

		bool isDirty();

		void resetDirtyState();

		std::string getName();

		virtual void save() = 0;
		virtual void load() = 0;

	protected:
		friend class MppParameters;
		bool dirty = false;
		std::string name;
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

	void load() override;
	void save() override;
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
	Parameter<size_t> volume = 0;
	Parameter<size_t> repeatCount = 0;
	Parameter<size_t> timeUpdateFrequency = 4;
	Parameter<size_t> spectrumAnalyzerBarAmount = 0;
	Parameter<bool> alwaysOnTop = false;
	Parameter<bool> hideTitleBar = false;
	Parameter<bool> hideByCloseButton = false;
	Parameter<bool> enableRightClickMenu = false;
	Parameter<bool> enableSystemTray = false;
	Parameter<bool> minimizeToSystemTray = false;
	Parameter<bool> hideApplicationIcon = false;

private:
	std::vector<ParameterBase *> parameters;
	void addParameter(ParameterBase &parameter, std::string name);
	QSettings * settings;
};

////
template<class T>
Parameter<T>::Parameter(const T& value) {
	this->value = value;
	dirty = false;
}

template<class T>
T Parameter<T>::operator=(const T& value) {
	if(this->value != value) {
		this->value = value;
		dirty = true;
	}
	return value;
}

//implicit conversion
template<class T>
Parameter<T>::operator T() const {
	return value;
}

#endif // MPPPARAMETERS_HPP
