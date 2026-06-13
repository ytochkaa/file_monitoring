QT += core
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = file_monitoring

SOURCES += \
    file_monitoring.cpp \
    monitoring.cpp \
    ConsoleLogger.cpp \
    FilePathHelper.cpp \
    directorywalker.cpp

HEADERS += \
    command_input.h \
    monitoring.h \
    ILogger.h \
    ConsoleLogger.h \
    FilePathHelper.h \
    directorywalker.h
