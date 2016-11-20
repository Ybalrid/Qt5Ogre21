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

class QtOgre21
{
public:

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
    std::tuple<Ogre::SceneManager*, Ogre::Camera*, Ogre::CompositorWorkspace*, Ogre::IdString>
    WidgetCreatedCallback(Ogre::RenderWindow* virtualWindow, size_t sceneIndex = 0);

    ///Methdo called just before creating a window
    void willCreateWindowHint();

    ///Return the selected API. OpenGL or DirextX 11
    RenderAPI getAPI();

    ///For Windows OpenGL only, return the context of the 1st window. Return null pointer if not relevent
    HGLRC getContext();

    ///Declare to the engine the location of the HLMS library
    void declareHlmsLibrary(const Ogre::String&& path);

    static void logToOgre(std::string message);

private:

    static QtOgre21* self;

    bool rendersystemReady;

    Ogre::String shadingLanguage;

    std::unique_ptr<Ogre::Root> root;

    static constexpr const char* const GL3PLUS_RENDERSYSTEM{ "OpenGL 3+ Rendering Subsystem" };
    static constexpr const char* const DIREXTX11_RENDERSYSTEM{ "Direct3D11 Rendering Subsystem" };

    uint8_t threads;

    std::vector<Ogre::SceneManager*> scenes;

    static size_t cameraCounter, workspaceCounter;

    RenderAPI usedAPI;

    HGLRC glContext;

    std::string hlmsPath;

};

#endif // QTOGRE21_H
