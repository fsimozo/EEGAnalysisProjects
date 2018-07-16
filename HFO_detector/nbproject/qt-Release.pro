# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Release/MinGW_Qt_x64-Windows
TARGET = HFO_detector
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += dll release 
PKGCONFIG +=
QT = core gui
SOURCES += FIRfilter.cpp hfoDetector.cpp methodResult.cpp resultElement.cpp writeMarkersText.cpp
HEADERS += FIRfilter.h hfoDetector.h methodResult.h resultElement.h writeMarkersText.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/MinGW_Qt_x64-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
