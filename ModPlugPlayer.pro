QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

# include(./SubModules/SpectrumAnalyzer/SpectrumAnalyzer.pri)

CONFIG += c++20

TARGET = "ModPlug Player"

win32 {
    TARGET = "ModPlug_Player"
    RC_FILE += MPP.rc
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH = $$PWD/src
INCLUDEPATH += $$PWD/SubModules/DSP/src
INCLUDEPATH += $$PWD/SubModules/SpectrumAnalyzerAnimator/src
INCLUDEPATH += $$PWD/SubModules/SpectrumAnalyzer/src
INCLUDEPATH += $$PWD/SubModules/GradientEditor/src

#INCLUDEPATH += $$PWD/SubModules/SpectrumAnalyzer/src
#DEPENDPATH += $$PWD/SubModules/SpectrumAnalyzer/src

SOURCES = $$files($$PWD/src/*.cpp, false)

SOURCES += $$files($$PWD/SubModules/GradientEditor/src/*.cpp, false)

HEADERS = $$files($$PWD/src/*.hpp, true)

HEADERS += $$files(./SubModules/GradientEditor/src/*.hpp, false)

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

mac {
    ICON = ./ui/Icons/MPP.icns
    OBJECTIVE_SOURCES += ./src/osSpecific/macOS/MacManager.mm
    LIBS += -framework Foundation -framework Cocoa
    HEADERS += ./src/OsSpecific/macOS/MacManager.h
}

RESOURCES += $$files(*.qrc, true)

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}
