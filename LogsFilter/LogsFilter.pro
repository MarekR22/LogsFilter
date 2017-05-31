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
    configs/acwinconfiguration.json \
    configs/acmacconfiguration.json
