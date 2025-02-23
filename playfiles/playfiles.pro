TEMPLATE = app
TARGET = playfiles.exe

CONFIG += console
CONFIG -= app_bundle

CONFIG -= qt
CONFIG += c++11
#QT += core gui
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG +=debug

HEADERS += \
  audiofilereader.h \
  audiocallback.h \
  CallbackData.h


SOURCES += main.cpp \
  audiofilereader.cpp \
  audiocallback.cpp \
  CallbackData.cpp

INCLUDEPATH += ${HOME}/local/include/rtaudio

LIBS += -L${HOME}/local/lib -lrtaudio
LIBS += -lsndfile

macx {
    INCLUDEPATH += /usr/local/include/
    INCLUDEPATH += /usr/local/include/rtaudio/
    LIBS += -L/usr/local/lib 


    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.15
}
