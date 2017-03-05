
message("The .pri for QtOgre has been found!");

#-------------------------------------------------
#
# Project created by QtCreator 2016-11-16T12:28:05
#
# For Qt5 and up only
#-------------------------------------------------

#QT       += core gui widgets

#TARGET = Qt5Ogre21
#TEMPLATE = app

#CONFIG += c++14 precompile_header

#VARIABLE TO BE SET BY THE USER WITH PATH TO THE OGRE SDK
#OGRE21SDK = "C:\YbalridSoftware\Ogre21\build\sdk"

message("Ogre21 SDK is $$OGRE21SDK");


OGRELIBPATH = $$OGRE21SDK\\lib\\


#include QtOgre pri here
#for now we're doing this by hand

INCLUDEPATH += $$QTOGRE

win32:INCLUDEPATH += $$OGRE21SDK\\include
win32:INCLUDEPATH += $$OGRE21SDK\\include\\OGRE

CONFIG(debug, debug|release) {
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreMain_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreOverlay_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreMeshLodGenerator_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsPbs_d.lib
    win32:LIBS += $$OGRELIBPATH\\debug\\OgreHlmsUnlit_d.lib
} else {
    win32:LIBS += $$OGRELIBPATH\\release\\OgreMain.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreOverlay.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreMeshLodGenerator.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsPbs.lib
    win32:LIBS += $$OGRELIBPATH\\release\\OgreHlmsUnlit.lib
}
    win32:LIBS += opengl32.lib

SOURCES += $$QTOGRE/qogreviewport.cpp \
    $$QTOGRE/qtogre21.cpp \

HEADERS  += $$QTOGRE/qogreviewport.h \
    $$QTOGRE/qtogre21.h \