#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T20:54:09
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += multimedia
QT       += axcontainer
CONFIG += release
Win32:CONFIG(debug, debug|release)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Butler
TEMPLATE = app


SOURCES += main.cpp\
        boot.cpp \
    login.cpp \
    database.cpp \
    regist.cpp \
    mainwindow.cpp \
    forget.cpp \
    add_user_data.cpp \
    excelengine.cpp \
    changeuserinfor.cpp

HEADERS  += boot.h \
    login.h \
    database.h \
    regist.h \
    mainwindow.h \
    forget.h \
    add_user_data.h \
    excelengine.h \
    definelist.h \
    changeuserinfor.h

FORMS    += boot.ui \
    login.ui \
    regist.ui \
    mainwindow.ui \
    forget.ui \
    add_user_data.ui \
    changeuserinfor.ui

RESOURCES += \
    sources.qrc
