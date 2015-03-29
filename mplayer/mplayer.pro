#-------------------------------------------------
#
# Project created by QtCreator 2015-03-19T22:48:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mplayer
TEMPLATE = app

LIBS += -lcurl

SOURCES += main.cpp\
        player.cpp \
    videoItem.cpp \
    httpClient.cpp

HEADERS  += player.h \
    videoItem.h \
    httpClient.h

FORMS    += player.ui

OTHER_FILES +=

