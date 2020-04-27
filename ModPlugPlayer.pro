QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# include(../SpectrumAnalyzer/SpectrumAnalyzer.pri)

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BeeperWIthCallback.cpp \
    LCDPanel.cpp \
    ModuleClasses.cpp \
    ModulePlayer.cpp \
    ModulePlayerThread.cpp \
    MppParameters.cpp \
    OptionButtons.cpp \
    PlayerControlButtons.cpp \
    SetupWindow.cpp \
    SineGenerator.cpp \
    TitleBar.cpp \
    main.cpp \
    PlayerWindow.cpp

HEADERS += \
    BeeperWIthCallback.hpp \
    Enums.hpp \
    LCDPanel.hpp \
    MathUtil.hpp \
    ModuleClasses.hpp \
    ModulePlayer.hpp \
    ModulePlayerThread.hpp \
    MppParameters.hpp \
    OptionButtons.hpp \
    PlayerControlButtons.hpp \
    PlayerWindow.hpp \
    SetupWindow.hpp \
    SineGenerator.hpp \
    TitleBar.hpp

FORMS += \
    LCDPanel.ui \
    OptionButtons.ui \
    PlayerControlButtons.ui \
    PlayerWindow.ui \
    SetupWindow.ui \
    TitleBar.ui

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

RESOURCES += \
    Resources.qrc
