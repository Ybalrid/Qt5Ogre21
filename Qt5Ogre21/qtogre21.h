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

class QtOgre21
{
public:

    enum class RenderAPI{OpenGL, DirectX11};

    QtOgre21(RenderAPI API, Ogre::String HlmsLibraryPath = ".");

    ~QtOgre21();

    static QtOgre21* instance();

    Ogre::MeshPtr loadFromV1Mesh(Ogre::v1::MeshPtr v1Mesh);

    Ogre::SceneManager* getScene(size_t index);

    std::tuple<Ogre::SceneManager*, Ogre::Camera*, Ogre::CompositorWorkspace*, Ogre::IdString >WidgetCreatedCallback(Ogre::RenderWindow* virtualWindow);

private:

    static QtOgre21* self;

    bool rendersystemReady;

    Ogre::String shadingLanguage;

    std::unique_ptr<Ogre::Root> root;

    static constexpr const char* const GL3PLUS_RENDERSYSTEM{ "OpenGL 3+ Rendering Subsystem" };

    uint8_t threads;

    std::vector<Ogre::SceneManager*> scenes;

    static size_t cameraCounter, workspaceCounter;

};

#endif // QTOGRE21_H
