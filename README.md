# Qt5Ogre21
Qt 5 integration of Ogre 2.1

Need a C++11 complient compier. Currently only tested with Visual Studio 2015 

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

### Easy integration of Ogre2 into a Qt project

This small library is suitable to be put inside your project via [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules)

```QMake
# Path to Ogre SDK. You can use a OGRE_HOME env var if you want. Check QMake documentation about environement variables
OGRE21SDK = "C:\YbalridSoftware\Ogre21\build\sdk"

#Path to where the QtOgre project files are. You can use $PWD to get the path of the folder of this .pro
QTOGRE = $${PWD}/../external/Qt5Ogre21/Qt5Ogre21/

#The magic include
include($$QTOGRE/Qt5Ogre21.pri)
```

Running on Windows and/or linux easily

### What to do in your code

Create an instance of the QtOgre21 object along side your QApplication. If you don't you can't create QOgreViewport widgets.

The QtOgre21 constructor takes 2 arguments : 

 - The rendering API to use, currently OpenGL or D3D11
 - The location of the HLMS library you are using. By defautl it's ./HLMS, meaning that you need to copy the Hlms folder from your ogre sdk there. Only "FileSystem" HLMS libs are supported right now, and it will try to load the PBS and the Unlit ones from the sample. If you're using a custom HLMS, you probably know how to load it and you can go and add that to the code of `QtOgre21::declareHlms()`

This object is a singleton class, you can access it via calling `QtOgre21::instance()`

Use QOgreViewport widgets as any other widget in Qt. You can have different scenes. Creating a new scene is done via the QtOgre21 object.

```C++
int main(int argc, char *argv[])
{
    //Initialize the Qt Application
    QApplication a(argc, argv);

    //Initialize the QtOgre system
    QtOgre21 qtOgre(QtOgre21::RenderAPI::OpenGL);
    
    //Here you can create widget with viewports (this is actually needed for initializing the rendering API) 
    //and .show() them!
    
    //Resource locations
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem");

    //HighLevelMaterialSystem shader libraries
    qtOgre.declareHlmsLibrary();

    //Initialize the resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
    
    //get the scene manager
    auto scene = qtOgre.getScene(0);
    
    //here you can do whatever you would do with an Ogre::SceneManager
    
    return a.exec()
}
```

More explicitely: When creating a first QOgreViewport the 1st time, Ogre will finish the RenderSystem intialization, has it needs a "window" object existing to setup the rendering pipeline.

Also, when doing that, it will create a first scene manager that will be the default one. 

An additional scene can be created when calling `QtOgre21::instance()->createNewScene();`. QtOgre21 store them as an array, and you will be able to get the scene by their index. 

The default scene is index `0`, the 1st additional one is `1`, etc... 

When calling the QOgreViewport constructor, you can specify a "scene manager index" as an argument, when doing this, it will render that scene. 

A viewport is automatically attached to a camera via a basic compositor, and ready to render. I'm planning to add a way to specify a compositor to use, but it's not done yet.

Viewports have 2 important methods : getCamera() and getSmgr(). Everything esle is usable as any Ogre feature. 



