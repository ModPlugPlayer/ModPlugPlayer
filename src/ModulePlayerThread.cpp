#include "ModulePlayerThread.hpp"
#include <QDebug>

void ModulePlayerThread::stop()
{
    if(!mp.isSongState(SongState::Loaded))
        return;
    if(!mp.isPlayerState(PlayerState::Stopped)) {
        mp.stop();
        mp.setPlayerState(PlayerState::Stopped);
    }
}

void ModulePlayerThread::play()
{
    if(!mp.isSongState(SongState::Loaded))
        return;
    if(mp.isPlayerState(PlayerState::Stopped)) {
        mp.play();
        mp.setPlayerState(PlayerState::Playing);
    }
    else if(mp.isPlayerState(PlayerState::Paused)) {
        mp.resume();
        mp.setPlayerState(PlayerState::Playing);
    }
}

void ModulePlayerThread::pause()
{
    if(!mp.isSongState(SongState::Loaded))
        return;
    if(mp.isPlayerState(PlayerState::Playing)) {
        mp.pause();
        mp.setPlayerState(PlayerState::Paused);
    }
    else if(mp.isPlayerState(PlayerState::Paused)) {
        mp.resume();
        mp.setPlayerState(PlayerState::Playing);
    }
}

void ModulePlayerThread::open(QString filePath){
    if(!mp.isPlayerState(PlayerState::Stopped)) {
        mp.stop();
    }
    mp.open(filePath.toStdString(), 2048, 1024, SampleRate::Hz44100);
    if(!filePath.isEmpty()) {
        this->start();
        qDebug()<<filePath<<" Loaded";
    }
    if(mp.isPlayerState(PlayerState::Playing)) {
        mp.play();
        qDebug()<<"Playing";
        mp.setPlayerState(PlayerState::Playing);
    }
    else
        mp.setPlayerState(PlayerState::Stopped);
    mp.setSongState(SongState::Loaded);
}


void ModulePlayerThread::init()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ModulePlayerThread::threadLoop);
    timer->start(50);
//    qDebug()<<"init";
//    mp.open(filePath.toStdString(), 480, 64, SAMPLERATE::Hz22050);
    MppParameters mppParameters;
    mppParameters.setRepeatCount(-1);
    mppParameters.setInterpolationFilter(InterpolationFilter::LinearInterpolation);
    mppParameters.setTimeUpdateFrequency(60);
    mppParameters.setBarAmount(20);
    mp.mppParametersChanged(mppParameters);
    //mp.play();
    this->start();
}

void ModulePlayerThread::threadLoop(){
    qDebug()<<"thread loop";
}
