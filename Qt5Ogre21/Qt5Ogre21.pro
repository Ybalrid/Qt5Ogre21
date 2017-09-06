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

#VARIABLE TO BE SET BY THE USER WITH PATH TO THE OGRE SDK
OGRE21SDK = "C:\Users\arthu\ogre\build\sdk"

OGRELIBPATH = $$OGRE21SDK\\lib\\

#include QtOgre pri here
#for now we're doing this by hand

win32:INCLUDEPATH += $$OGRE21SDK\\include
win32:INCLUDEPATH += $$OGRE21SDK\\include\\OGRE
linux:INCLUDEPATH += /usr/local/include/OGRE
linux:LIBS += -L/usr/local/lib

CONFIG(debug, debug|release) {
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreMain_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreOverlay_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreMeshLodGenerator_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsPbs_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsUnlit_d.lib

    linux:LIBS += -lOgreMain_d
    linux:LIBS += -lOgreOverlay_d
    linux:LIBS += -lOgreMeshLodGenerator_d
    linux:LIBS += -lOgreHlmsPbs_d
    linux:LIBS += -lOgreHlmsUnlit_d

} else {
    win32:LIBS += $$OGRELIBPATH\\release\\OgreMain.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreOverlay.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreMeshLodGenerator.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsPbs.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsUnlit.lib

    linux:LIBS += -lOgreMain
    linux:LIBS += -lOgreOverlay
    linux:LIBS += -lOgreMeshLodGenerator
    linux:LIBS += -lOgreHlmsPbs
    linux:LIBS += -lOgreHlmsUnlit

}
    win32:LIBS += opengl32.lib

SOURCES += test.cpp\
        qogreviewport.cpp \
    qtogre21.cpp \
    somecustomwidget.cpp

HEADERS  += qogreviewport.h \
    qtogre21.h \
    somecustomwidget.h
