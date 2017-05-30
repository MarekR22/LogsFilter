#-------------------------------------------------
#
# Project created by QtCreator 2017-05-26T21:22:25
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogsFilter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logsdatamodel.cpp \
    logsformatconfiguration.cpp \
    timezonetool.cpp

HEADERS  += mainwindow.h \
    logsdatamodel.h \
    logsformatconfiguration.h \
    timezonetool.h

FORMS    += mainwindow.ui

DISTFILES += \
    acwinconfiguration.json \
    acmacconfiguration.json
