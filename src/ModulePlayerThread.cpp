#include "ModulePlayerThread.hpp"
#include <QDebug>

void ModulePlayerThread::stop()
{
    if(this->playerState != PLAYERSTATE::STOPPED) {
        mp.stop();
        playerState = PLAYERSTATE::STOPPED;
    }
}

void ModulePlayerThread::play()
{
    if(this->playerState == PLAYERSTATE::STOPPED) {
        mp.play();
        playerState = PLAYERSTATE::PLAYING;
    }
    else if(this->playerState == PLAYERSTATE::PAUSED) {
        mp.resume();
        playerState = PLAYERSTATE::PLAYING;
    }
}

void ModulePlayerThread::pause()
{
    if(this->playerState == PLAYERSTATE::PLAYING) {
        mp.pause();
        playerState = PLAYERSTATE::PAUSED;
    }
    else if(this->playerState == PLAYERSTATE::PAUSED) {
        mp.resume();
        playerState = PLAYERSTATE::PLAYING;
    }
}

void ModulePlayerThread::open(QString filePath){
    if(playerState != PLAYERSTATE::STOPPED) {
        mp.stop();
    }
    this->filePath = filePath;
    if(!this->filePath.isEmpty()) {
        this->start();
        qDebug()<<filePath<<" Loaded";
    }
    if(playerState == PLAYERSTATE::PLAYING) {
        mp.play();
        qDebug()<<"Playing";
        //playerState = PLAYERSTATE::PLAYING;
    }
    else
        playerState = PLAYERSTATE::STOPPED;
}


void ModulePlayerThread::init()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ModulePlayerThread::threadLoop);
    timer->start(50);
//    qDebug()<<"init";
    mp.open(filePath.toStdString(), 480, 64, SAMPLERATE::Hz22050);
    MppParameters mppParameters;
    mppParameters.setRepeatCount(-1);
    mppParameters.setInterpolationFilter(INTERPOLATIONFILTER::LINEAR_INTERPOLATION);
    mppParameters.setTimeUpdateFrequency(60);
    mppParameters.setBarAmount(20);
    mp.mppParametersChanged(mppParameters);
    //mp.play();
    playerState = PLAYERSTATE::STOPPED;
    songState = SONGSTATE::NOT_LOADED;
    this->start();
}

void ModulePlayerThread::threadLoop(){
    qDebug()<<"thread loop";
}
