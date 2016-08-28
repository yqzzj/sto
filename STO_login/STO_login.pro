#-------------------------------------------------
#
# Project created by QtCreator 2016-08-19T10:10:07
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STO_login
TEMPLATE = app

DESTDIR = ../stoTarget

SOURCES += main.cpp\
        widget.cpp \
    signupwidget.cpp \
    database/databasehandle.cpp \

HEADERS  += widget.h \
    signupwidget.h \
    GlobalData.h \
    userdefine/myhelper.h \
    database/DataBase.h \
    database/databasehandle.h \

FORMS    += widget.ui \
    signupwidget.ui \
    clausedialog.ui
