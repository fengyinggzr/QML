#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T10:40:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = portComm
TEMPLATE = app


SOURCES += main.cpp\
        portcomm.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialbase.cpp

HEADERS  += portcomm.h \
    win_qextserialport.h \
    qextserialport.h \
    qextserialbase.h

FORMS    += portcomm.ui
