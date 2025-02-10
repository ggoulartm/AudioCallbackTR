TEMPLATE = app
TARGET = playsine.exe

CONFIG += console
CONFIG -= app_bundle

CONFIG -= qt
CONFIG += c++11

#QT += core gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += playsine.cpp

INCLUDEPATH += ${HOME}/local/include/rtaudio

LIBS += -L${HOME}/local/lib -lrtaudio

macx {
    INCLUDEPATH += /usr/local/include/
    INCLUDEPATH += /usr/local/include/rtaudio/
    LIBS += -L/usr/local/lib

    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.15
}
