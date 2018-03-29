QT += core dbus
QT -= gui

CONFIG += c++11

TARGET = backend
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    backend.cpp \
    crc.cpp \
    ecc.cpp \
    mod.cpp \
    channel.cpp \
    ../shared/oop_adaptor.cpp \
    ../shared/oop_interface.cpp

HEADERS += \
    backend.h \
    crc.h \
    ecc.h \
    mod.h \
    channel.h \
    ../shared/oop_adaptor.h \
    ../shared/oop_definitions.h \
    ../shared/oop_interface.h

DISTFILES += \
    ../shared/oop.xml

