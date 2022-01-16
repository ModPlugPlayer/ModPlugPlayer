#Build script of ModPlug Player
#Copyright (C) 2020 Volkan Orhan

#This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

#You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(ModPlugPlayer VERSION 0.1 LANGUAGES CXX)

if(APPLE)
    enable_language(OBJCXX)
endif()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
if(NOT PKG_CONFIG_FOUND)
  message(FATAL_ERROR "pkg-config not found!" )
endif()
#find_package(PortAudio)
#find_package(PortAudioCpp)





pkg_check_modules(FFTW REQUIRED IMPORTED_TARGET fftw3)
pkg_check_modules(PortAudio REQUIRED IMPORTED_TARGET portaudio-2.0)
pkg_check_modules(PortAudioCPP REQUIRED IMPORTED_TARGET portaudiocpp)
pkg_check_modules(OpenMPT REQUIRED IMPORTED_TARGET libopenmpt)

#pkg_search_module(PORTAUDIO REQUIRED portaudio-2.0)
#pkg_search_module(PORTAUDIOCPP REQUIRED portaudiocpp)

#include_directories(${PORTAUDIO_INCLUDE_DIRS})
#include_directories(${PORTAUDIOCPP_INCLUDE_DIRS})

include_directories(src)
include_directories(SubModules/DSP/src)
include_directories(SubModules/SpectrumAnalyzerAnimator/src)
include_directories(SubModules/SpectrumAnalyzer/src)
include_directories(SubModules/GradientEditor/src)

if(APPLE)
    file(GLOB ModPlugPlayerObjCppSrc
        "./src/OsSpecific/macOS/*.mm"
        "./src/OsSpecific/macOS/*.h"
    )

    SET_SOURCE_FILES_PROPERTIES(
        ${ModPlugPlayerObjCppSrc}
        PROPERTIES LANGUAGE OBJCXX
    )
endif()
file(GLOB ModPlugPlayerSrc
    "./src/*.cpp"
    "./src/*.hpp"
    "./ui/*.ui"
    "./ui/*.qrc"
)

file(GLOB SpectrumAnalyzerSrc
    "SubModules/SpectrumAnalyzer/src/*.cpp"
    "SubModules/SpectrumAnalyzer/src/*.hpp"
)

file(GLOB DspSrc
    "SubModules/DSP/src/*.cpp"
    "SubModules/DSP/src/*.hpp"
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
    "SubModules/SpectrumAnalyzerAnimator/src/*.cpp"
    "SubModules/SpectrumAnalyzerAnimator/src/*.hpp"
)

file(GLOB GradientEditorSrc
    "SubModules/GradientEditor/src/*.cpp"
    "SubModules/GradientEditor/src/*.hpp"
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    "src"
    "ui"
    "SubModules/SpectrumAnalyzer/src"
    "SubModules/SpectrumAnalyzerAnimator/src"
    "SubModules/DSP/src"
    "SubModules/GradientEditor/src"
    )


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ModPlugPlayer
        MANUAL_FINALIZATION
        ${ModPlugPlayerSrc}
        ${ModPlugPlayerObjCppSrc}
        ${SpectrumAnalyzerSrc}
        ${DspSrc}
        ${SpectrumAnalyzerAnimatorSrc}
        ${GradientEditorSrc}
    )
else()
    add_executable(ModPlugPlayer
        ${ModPlugPlayerSrc}
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
       #${PORTAUDIO_LIBRARIES}
       #${PORTAUDIOCPP_LIBRARIES}
       PkgConfig::PortAudio
       PkgConfig::PortAudioCPP
       PkgConfig::OpenMPT
      )

#target_compile_options(ModPlugPlayer PUBLIC ${PORTAUDIO_CFLAGS_OTHER} ${PORTAUDIOCPP_CFLAGS_OTHER})


set_target_properties(ModPlugPlayer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ModPlugPlayer)
endif()
