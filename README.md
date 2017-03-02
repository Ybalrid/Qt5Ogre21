# Qt5Ogre21
Qt 5 integration of Ogre 2.1

![Screenshot](/Screenshot.png)

The goal is to have multiple viewports of Ogre either running separate or multiple different scenes.

Design idea
  - Initialization of Ogre Root object, and of the HLMS with lifetime tied to a sigle object
  - Integration of Qt events with Ogre
  - a QOgreViewport widget class containing an Ogre "external render window"
  - Possibility to refresh the viewport at repaint, or at a "user chosen" framerate (30, 60fps)
  
*this project is not intended to render games, but to provide an usefull utility for integrating a 3D view to a desktop application, like an editor*
 
Since Ogre2 is fairly new, some Ogre1->Ogre2 transition facilities have to be in place, like the runtime convertion of v1 .mesh `MeshSerializer` objects to the new .mesh format. 

## How to use

Create an instance of the QtOgre21 object along side your QApplication. If you don't you can't create QOgreViewport widgets.

Use QOgreViewport widgets as any other widget in Qt. You can have different scenes. Creating a new scene is done via the QtOgre21 object.

This object is a singleton class, you can access it via calling `QtOgre21::instance()`


## Easy integration of Ogre2 into a Qt project

Setting a variable and including a ".pri" (TODO it's not done yet. You can get the classes and the .pro configs by hand) file in your main .pro and use Qt's own build system (QMake)

Running on Windows and/or linux easily

