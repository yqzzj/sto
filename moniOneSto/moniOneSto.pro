#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T16:48:32
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = moniOneSto
TEMPLATE = app

DESTDIR = ../stoTarget

SOURCES += main.cpp\
        Widget.cpp \
    stockData.cpp

HEADERS  += Widget.h \
    stockData.h

FORMS    += Widget.ui
