# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Debug/MinGW_Qt_x64-Windows
TARGET = dfa
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += dll debug 
PKGCONFIG +=
QT = core gui
SOURCES += FIRfilter.cpp dfaAnalysis.cpp dfa_plugin.cpp methodResult.cpp resultElement.cpp simulatedHfoGenerator.cpp writeMarkersText.cpp
HEADERS += FIRfilter.h dfaAnalysis.h dfa_plugin.h methodResult.h resultElement.h simulatedHfoGenerator.h writeMarkersText.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/MinGW_Qt_x64-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += ../../../../../fftw-3.3.3-dll64 
LIBS += -lfftw3-3  
