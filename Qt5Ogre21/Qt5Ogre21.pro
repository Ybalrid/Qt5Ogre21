#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T12:28:05
#
# For Qt5 and up only
#-------------------------------------------------

QT       += core gui widgets

TARGET = Qt5Ogre21
TEMPLATE = app

CONFIG += c++14 precompile_header

OGRE21SDK = "C:\Oculus\Ogre\ogre\build\sdk"

OGRELIBPATH = $$OGRE21SDK\\lib\\

#include QtOgre pri here

#for now we're doing this by hand


win32:INCLUDEPATH += $$OGRE21SDK\\include
win32:INCLUDEPATH += $$OGRE21SDK\\include\\OGRE

CONFIG(debug, debug|release) {
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreMain_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsPbs_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsUnlit_d.lib
} else {
    win32:LIBS += $$OGRELIBPATH\\release\\OgreMain.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsPbs.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsUnlit.lib
}




SOURCES += main.cpp\
        qogreviewport.cpp \
    qtogre21.cpp

HEADERS  += qogreviewport.h \
    qtogre21.h
