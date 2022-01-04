#Build script of ModPlug Player
#Copyright (C) 2020 Volkan Orhan

#This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

#You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(ModPlugPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Gui REQUIRED)
find_package(QT NAMES Qt6 COMPONENTS Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Svg REQUIRED)

find_package(PkgConfig REQUIRED)
#find_package(portaudio)
#find_package(portaudiocpp)

pkg_check_modules(FFTW REQUIRED IMPORTED_TARGET fftw3)
#pkg_check_modules(PortAudio REQUIRED IMPORTED_TARGET portaudio-2.0)
#pkg_check_modules(PortAudioCPP REQUIRED IMPORTED_TARGET portaudiocpp)
pkg_check_modules(OpenMPT REQUIRED IMPORTED_TARGET libopenmpt)

include_directories(../DSP/src)
include_directories(../SpectrumAnalyzerAnimator/src)
include_directories(../SpectrumAnalyzer/src)

file(GLOB ModPlugPlayerUiSrc
    "src/*.cpp"
)

file(GLOB ModPlugPlayerUiHdr
    "src/*.hpp"
)

file(GLOB ModPlugPlayerUi
    "ui/*.ui"
)

file(GLOB ModPlugPlayerResources
    "ui/*.qrc"
)

file(GLOB SpectrumAnalyzerSrc
    "../SpectrumAnalyzer/src/*.cpp"
    "../SpectrumAnalyzer/src/*.hpp"
)

file(GLOB DspSrc
    "../DSP/src/*.cpp"
    "../DSP/src/*.hpp"
)

#
#set(PROJECT_SOURCES
#    ${ModPlugPlayerUiSrc}
#    ${ModPlugPlayerUiHdr}
#    ${ModPlugPlayerUi}
#    ${ModPlugPlayerResources}
#    ${SpectrumAnalyzerSrc}
#    ${DspSrc}
#    ${SpectrumAnalyzerAnimatorSrc}
#    ${GradientEditorSrc}
#)


file(GLOB SpectrumAnalyzerAnimatorSrc
    "../SpectrumAnalyzerAnimator/src/*.cpp"
    "../SpectrumAnalyzerAnimator/src/*.hpp"
)

file(GLOB GradientEditorSrc
    "../GradientEditor/src/*.cpp"
    "../GradientEditor/src/*.hpp"
)

qt6_wrap_ui( UI_HEADERS ${ModPlugPlayerUi})
qt6_wrap_cpp( UI_MOC_SRC ${ModPlugPlayerUiHdr})
qt6_wrap_cpp( UI_MOC_SRC2 ${ModPlugPlayerUiSrc})
qt6_add_resources(UI_RESOURCES ${ModPlugPlayerResources})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ModPlugPlayer
        MANUAL_FINALIZATION
        ${ModPlugPlayerUiSrc}
        ${ModPlugPlayerUiHdr}
        ${ModPlugPlayerUi}
        ${UI_HEADERS}
        ${UI_MOC_SRC}
        ${UI_MOC_SRC2}
        ${UI_RESOURCES}
        ${SpectrumAnalyzerSrc}
        ${DspSrc}
        ${SpectrumAnalyzerAnimatorSrc}
        ${GradientEditorSrc}
    )
else()
    add_executable(ModPlugPlayer
        ${ModPlugPlayerSrc}
        ${ModPlugPlayerUi}
        ${SpectrumAnalyzerSrc}
        ${DspSrc}
        ${SpectrumAnalyzerAnimatorSrc}
        ${GradientEditorSrc}
    )
endif()


target_link_libraries(ModPlugPlayer PRIVATE
       Qt${QT_VERSION_MAJOR}::Widgets
       Qt${QT_VERSION_MAJOR}::Core
       Qt${QT_VERSION_MAJOR}::Gui
       Qt${QT_VERSION_MAJOR}::Svg
       PkgConfig::FFTW
       #PkgConfig::PortAudio
       #PkgConfig::PortAudioCPP
       PkgConfig::OpenMPT
      )

set_target_properties(ModPlugPlayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ModPlugPlayer)
endif()
