#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T23:54:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server_module
TEMPLATE = app


SOURCES += main.cpp\
        topiserver.cpp \
    controlpanel.cpp \
    picturebox.cpp \
    mainwindow.cpp

HEADERS  += topiserver.h \
    controlpanel.h \
    picturebox.h \
    mainwindow.h

FORMS    += topiserver.ui \
    controlpanel.ui \
    mainwindow.ui

QT += network
