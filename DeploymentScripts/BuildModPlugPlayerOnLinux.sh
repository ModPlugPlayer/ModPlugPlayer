#!/bin/bash
sudo apt install \
             libopenmpt-dev \
             build-essential \
             libfftw3-bin libfftw3-dev \
             libportaudio2 portaudio19-dev libportaudiocpp0 \
             libpulse-dev \
             libsndfile1-dev \
             libglu1-mesa-dev freeglut3-dev mesa-common-dev \
             cmake ninja-build \
             libmpg123-dev \
             libboost-all-dev \
             qt6-base-dev qt6-tools-dev qt6-svg-dev

cd ..
mkdir MPPBinary
cmake -B ./MPPBinary -DCMAKE_BUILD_TYPE=Release
cd MPPBinary
cmake –build ../MPPBinary –config Release
make
mv ModPlug\ Player ~/
cd ~
open .
