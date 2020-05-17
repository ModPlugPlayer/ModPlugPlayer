#ifndef MODULEPLAYERTHREAD_HPP
#define MODULEPLAYERTHREAD_HPP
#include <QThread>
#include <QTimer>
#include "ModulePlayer.hpp"
class ModulePlayerThread : public QThread
{
    Q_OBJECT
public:
    ModulePlayer mp;


    void run() override {
        QString result;
        //this->open(filePath);
        this->init();
        emit resultReady(result);
    }
signals:
    void resultReady(const QString &s);
    void fileOpened();

public slots:
    void stop();
    void play();
    void pause();
    void init();
    void threadLoop();
    void open(QString filePath);
private:
    QTimer *timer;
};

#endif // MODULEPLAYERTHREAD_HPP
