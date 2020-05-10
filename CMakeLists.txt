cmake_minimum_required(VERSION 3.5)

project(untitled5 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)







file(GLOB MODPLUGPLAYER_SRC
    "src/*.hpp"
    "src/*.cpp"
)

file(GLOB SPECTRUMANALYZER_TEST
    ${SPECTRUMANALYZER_SRC}
    "test/*.hpp"
    "test/*.cpp"
    "test/*.ui"
)

add_library(SpectrumAnalyzer STATIC
    ${SPECTRUMANALYZER_SRC}
)

add_executable(SpectrumAnalyzerExe
    ${SPECTRUMANALYZER_TEST}
)


include_directories("../DSP/src")




# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(untitled5 SHARED
    main.cpp
    MainWindow.cpp
    MainWindow.hpp
    MainWindow.ui
  )
else()
  add_executable(untitled5
    main.cpp
    MainWindow.cpp
    MainWindow.hpp
    MainWindow.ui
  )
endif()

target_link_libraries(untitled5 PRIVATE Qt5::Widgets)
