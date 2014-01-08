#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T22:14:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dilby
TEMPLATE = app


SOURCES += main.cpp\
           dilby.cpp \
           downloader.cpp

HEADERS  += dilby.h \
            downloader.h

FORMS    += dilby.ui

RESOURCES += res/Icons.qrc

RC_FILE = res/Dilby.rc
