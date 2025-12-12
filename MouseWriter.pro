#-------------------------------------------------
#
# Project created by QtCreator 2021-09-12T15:27:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MouseWriter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    read_thread.cpp

HEADERS  += mainwindow.h \
    CharReco.h \
    CharRecoSvr.h \
    HSEngine.h\
    thread.h\
    hidapi.h

FORMS    += mainwindow.ui



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lCharReco
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lCharReco
else:unix: LIBS += -L$$PWD/./ -lCharReco
win32: LIBS += -L$$PWD/./ -lhidapi
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.


