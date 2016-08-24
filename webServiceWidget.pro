#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T15:27:29
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sto
TEMPLATE = app

RCC_DIR = ../tmp/rcc
UI_HEADERS_DIR = ../tmp/ui
MOC_DIR = ../tmp/moc
OBJECTS_DIR = ../tmp/obj
DESTDIR = ../stoTarget


SOURCES += main.cpp\
        Widget.cpp \
    webServiceForm.cpp \
    stockData.cpp \
    globalDefine.cpp \
    MainWindow.cpp \
    mainwidget.cpp \
    titlewidget.cpp \
    toolwidget.cpp \
    toolbutton.cpp \
    stackedwidget.cpp \
    pushbutton.cpp \
    sysbutton.cpp

HEADERS  += Widget.h \
    webServiceForm.h \
    stockData.h \
    globalDefine.h \
    MainWindow.h \
    mainwidget.h \
    titlewidget.h \
    toolwidget.h \
    toolbutton.h \
    stackedwidget.h \
    pushbutton.h \
    sysbutton.h

FORMS    += Widget.ui \
    MainWindow.ui

OTHER_FILES +=

RESOURCES += \
    a.qrc

RC_FILE = stologo.rc
