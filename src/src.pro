#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T16:50:48
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myParser
TEMPLATE = app
QMAKE_CXXFLAGS += "-std=c++11"

win32|win64{
    RC_FILE=  index.rc
    OTHER_FILES+= index.rc
    DISTFILES += index.rc
}

SOURCES += main.cpp\
    WebKit/webview.cpp \
    WebKit/webpage.cpp \
    global.cpp \
    webengine.cpp

HEADERS  += \
    WebKit/webview.h \
    WebKit/webpage.h \
    global.h \
    webengine.h

DISTFILES += \
    index.rc
