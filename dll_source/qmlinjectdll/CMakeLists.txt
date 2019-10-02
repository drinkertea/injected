cmake_minimum_required(VERSION 3.10)

project(qt_dll_test)

set(CMAKE_PREFIX_PATH "C:\\Qt\\5.13.0\\msvc2017_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SOURCE_LIB dllmain.cpp qqq.h qqq.cpp)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_library(pyautoqt64 SHARED ${SOURCE_LIB})
    qt5_use_modules(pyautoqt64 Core)
    qt5_use_modules(pyautoqt64 Qml)
    qt5_use_modules(pyautoqt64 Widgets)
else ()
    add_library(pyautoqt32 SHARED ${SOURCE_LIB})
    qt5_use_modules(pyautoqt32 Core)
    qt5_use_modules(pyautoqt32 Qml)
    qt5_use_modules(pyautoqt32 Widgets)
endif ()