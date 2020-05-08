QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# include(../SpectrumAnalyzer/SpectrumAnalyzer.pri)

CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "./src"
INCLUDEPATH += "../DSP/src"

SOURCES = $$files(*.cpp, true)

HEADERS = $$files(*.hpp, true)

FORMS += $$files(*.ui, true)

TRANSLATIONS += \
    ModPlugPlayer_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

mac {
  PKG_CONFIG = /usr/local/bin/pkg-config
}

CONFIG += link_pkgconfig
PKGCONFIG += libopenmpt portaudio-2.0 portaudiocpp fftw3

macx{
    OBJECTIVE_SOURCES += MacManager.mm
    LIBS += -framework Foundation -framework Cocoa
    HEADERS += MacManager.h
}

RESOURCES += $$files(*.qrc, true)
