QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

# include(../SpectrumAnalyzer/SpectrumAnalyzer.pri)

CONFIG += c++20

TARGET = "ModPlug Player"

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "./src"
INCLUDEPATH += "../DSP/src"
INCLUDEPATH += "../SpectrumAnalyzerAnimator/src"
#INCLUDEPATH += "../SpectrumAnalyzer/src"

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
    ICON = ./ui/Icons/MPP.icns
    PKG_CONFIG = /usr/local/bin/pkg-config
}

CONFIG += link_pkgconfig
PKGCONFIG += libopenmpt portaudio-2.0 portaudiocpp fftw3

macx{
    OBJECTIVE_SOURCES += ./src/MacManager.mm
    LIBS += -framework Foundation -framework Cocoa
    HEADERS += ./src/MacManager.h
}

RESOURCES += $$files(*.qrc, true)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Build/build-SpectrumAnalyzer-Desktop_x86_darwin_generic_mach_o_64bit-Release/ -lSpectrumAnalyzer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Build/build-SpectrumAnalyzer-Desktop_Qt_6_0_0_clang_64bit-Debug/ -lSpectrumAnalyzer
else:unix: LIBS += -L$$PWD/../Build/build-SpectrumAnalyzer-Desktop_Qt_6_0_0_clang_64bit-Release/ -lSpectrumAnalyzer

INCLUDEPATH += $$PWD/../SpectrumAnalyzer/src
DEPENDPATH += $$PWD/../SpectrumAnalyzer/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Build/build-SpectrumAnalyzer-Desktop_Qt_6_0_0_clang_64bit-Release/release/libSpectrumAnalyzer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-SpectrumAnalyzer-Desktop_x86_darwin_generic_mach_o_64bit-Release/debug/libSpectrumAnalyzer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../build-SpectrumAnalyzer-Desktop_x86_darwin_generic_mach_o_64bit-Release/release/SpectrumAnalyzer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../build-SpectrumAnalyzer-Desktop_x86_darwin_generic_mach_o_64bit-Release/debug/SpectrumAnalyzer.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Build/build-SpectrumAnalyzer-Desktop_Qt_6_0_0_clang_64bit-Release/libSpectrumAnalyzer.a
