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
    header-gen.sh \
    images/ai/ai-horizon2.png \
    images/ai/ai-horizon3.png \
    images/ai/ai-plane.png \
    images/ai/k-ai-offset.png \
    images/alt/alt-inhg.png \
    images/alt/alt-plate.png \
    images/alt/k-alt-inhg.png \
    images/asi/asi-plate.png \
    images/common/frame.png \
    images/hi/hi-plane.png \
    images/hi/hi-plate.png \
    images/hi/k-hi-bug.png \
    images/hi/k-hi-head.png \
    images/radio/k-khz.png \
    images/radio/k-mhz.png \
    images/radio/k-vol.png \
    images/rpm/rpm-plate.png \
    images/tsi/tsi-plane.png \
    images/tsi/tsi-plate.png \
    images/vor/k-vor-obs.png \
    images/vor/vor-gs-flag.png \
    images/vor/vor-plate.png \
    images/vor/vor-ring.png \
    images/vor/vor-to-from-flag.png \
    images/vsi/vsi-plate.png \
    images/vsi/vsi-plate1.png
