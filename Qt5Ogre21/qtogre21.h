#ifndef QTOGRE21_H
#define QTOGRE21_H

#include <QDebug>

#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <thread>
#include <unordered_map>
#include <tuple>

#include <OGRE/Ogre.h>
#include <OGRE/Compositor/OgreCompositorManager2.h>
#include <OGRE/Compositor/OgreCompositorWorkspace.h>
#include <OGRE/OgreMeshManager2.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreItem.h>
#include <OGRE/OgreHlms.h>
#include <OGRE/OgreHlmsManager.h>
#include <OGRE/Hlms/Pbs/OgreHlmsPbs.h>
#include <OGRE/Hlms/Unlit/OgreHlmsUnlit.h>

//Necessary on Non windows platform :
#ifndef _WIN32
    using HGLRC = void*;
#endif

class QtOgre21
{
public:

    using WidgetCreationCallbackTuple = std::tuple<Ogre::SceneManager*, Ogre::Camera*, Ogre::CompositorWorkspace*, Ogre::IdString>;
    ///Usable Rendering APIs
    enum class RenderAPI{OpenGL, DirectX11};

    ///Class constructor. this class is singleton
    QtOgre21(RenderAPI API, Ogre::String HlmsLibraryPath = "HLMS");

    ///Class destructor
    ~QtOgre21();

    ///Return the instance of this singleton class
    static QtOgre21* instance();

    ///Helper function that return a V2 mesh from a V1 mesh. V1 mesh will be unallocated
    Ogre::MeshPtr loadFromV1Mesh(Ogre::v1::MeshPtr v1Mesh);

    ///Helper function that load the V1 mesh with the given name of a .mesh file known to the resource manager
    Ogre::MeshPtr loadFromV1Mesh(Ogre::String name);

    ///Get the `index` scene
    Ogre::SceneManager* getScene(size_t index);

    ///Create a Scene and append it to the Scene List
    void createNewScene();

    ///Method called by each viewport widget when it is showed.
    WidgetCreationCallbackTuple
    WidgetCreatedCallback(Ogre::RenderWindow* virtualWindow, size_t sceneIndex = 0);

    ///Methdo called just before creating a window
    void willCreateWindowHint();

    ///Return the selected API. OpenGL or DirextX 11
    RenderAPI getAPI();

    ///For Windows OpenGL only, return the context of the 1st window. Return null pointer if not relevent
    HGLRC getContext();

    ///Declare to the engine the location of the HLMS library
    void declareHlmsLibrary();

    ///Write a message to Ogre's log
    static void logToOgre(std::string message);

    ///Set the AA level
    void setAALevel(uint8_t AA);

    ///Get the AA level
    uint8_t getAALevel();

    ///Set the default background color for viewports created after this one:
    void setDefaultBackgroundColor(const Ogre::ColourValue &c);

    ///Set the name of a compositor to use. You are responsible for actually loading that compositor resources
    void setCustomCompositor(const Ogre::String& name);

private:

    Ogre::String customCompositor;

    ///The AA level
    uint8_t AALevel;

    ///Singleton pointer
    static QtOgre21* self;

    ///Name of the SL to use (for finding the correct HLMS to use)
    Ogre::String shadingLanguage;

    ///Smart pointer to the Ogre::Root, for automatic Ogre cleanup
    std::unique_ptr<Ogre::Root> root;

    ///Name of the OpenGL RenderSystem
    static constexpr const char* const GL3PLUS_RENDERSYSTEM{ "OpenGL 3+ Rendering Subsystem" };

    ///Name of the DirectX 11 RenderSystem
    static constexpr const char* const DIREXTX11_RENDERSYSTEM{ "Direct3D11 Rendering Subsystem" };

    ///Number of threads the SMGRs will use to traverse and cull the geometry
    uint8_t threads;

    ///List of the scenes to use
    std::vector<Ogre::SceneManager*> scenes;

    ///Static counters for uniques names
    static size_t cameraCounter, workspaceCounter;

    ///The API we were requested to use.
    RenderAPI usedAPI;

    ///Windows OpenGL context
    HGLRC glContext;

    ///Path to the HLMS library folder
    std::string hlmsPath;

    ///Defautl background color for new viewports
    Ogre::ColourValue defaultBackgroundColor;

};

#endif // QTOGRE21_H
