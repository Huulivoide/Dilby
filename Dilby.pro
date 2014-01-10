#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T22:14:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = Dilby
TEMPLATE = app
VERSION = 0.1.0


SOURCES += main.cpp\
           dilby.cpp \
           downloader.cpp \
           settingsdialog.cpp

HEADERS  += dilby.h \
            downloader.h \
            settingsdialog.h

FORMS    += dilby.ui \
            settingsdialog.ui

RESOURCES += res/Icons.qrc \
             i18n/Translations.qrc

TRANSLATIONS = i18n/fi_FI.ts

RC_FILE = res/Dilby.rc

OTHER_FILES += LICENSE
