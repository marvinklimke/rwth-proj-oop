#-------------------------------------------------
#
# Project created by QtCreator 2017-05-21T13:31:40
#
#-------------------------------------------------

QT       += core gui dbus

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = frontend
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    frontend.cpp \
    myimage.cpp \
    ../shared/oop_adaptor.cpp \
    ../shared/oop_interface.cpp

HEADERS  += mainwindow.h \
    frontend.h \
    myimage.h \
    ../shared/oop_adaptor.h \
    ../shared/oop_definitions.h \
    ../shared/oop_interface.h

FORMS    += mainwindow.ui

DISTFILES += \
    ../shared/oop.xml
