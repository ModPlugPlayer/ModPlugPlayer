#ifndef BEEPERWITHCALLBACK_HPP
#define BEEPERWITHCALLBACK_HPP
#include <string>
#include "Enums.hpp"
#include <portaudiocpp/PortAudioCpp.hxx>
#include <QObject>

class BeeperWIthCallback : public QObject
{
    Q_OBJECT
public:
    BeeperWIthCallback();
    ~BeeperWIthCallback();
    int open();
    int play();
signals:
    void timech(int x);
};

#endif // BEEPERWITHCALLBACK_HPP
