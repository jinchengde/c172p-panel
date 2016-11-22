#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T13:01:49
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c172p-panel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    attitudeindicator.cpp \
    airspeedindicator.cpp \
    turnslipindicator.cpp \
    altitudeindicator.cpp \
    headingindicator.cpp \
    navigationindicator.cpp \
    verticalspeedindicator.cpp \
    fgdata.cpp \
    config.cpp \
    datanames.cpp \
    rpm.cpp \
    knob.cpp \
    radio.cpp \
    dme.cpp

HEADERS  += mainwindow.h \
    attitudeindicator.h \
    airspeedindicator.h \
    turnslipindicator.h \
    altitudeindicator.h \
    headingindicator.h \
    navigationindicator.h \
    verticalspeedindicator.h \
    imgres.h \
    logutil.h \
    fgdata.h \
    config.h \
    datanames.h \
    rpm.h \
    knob.h \
    radio.h \
    dme.h

FORMS    += mainwindow.ui

DISTFILES += \
    README \
    fg/c172p-panel-in.xml \
    fg/c172p-panel-out.xml \
    header-gen.sh
