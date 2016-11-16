#include "qtogre21.h"

using namespace std;

QtOgre21* QtOgre21::self(nullptr);
size_t QtOgre21::cameraCounter(0);
size_t QtOgre21::workspaceCounter(0);

QtOgre21::QtOgre21(RenderAPI API, Ogre::String HlmsLibraryPath) :
    root{nullptr},
    rendersystemReady{false}
{
    if(self)
        throw std::runtime_error("Cannot instanciate 2 QtOgre21 objects. This is a singleton class.");


    threads = uint8_t(std::thread::hardware_concurrency());
    qDebug() << "Detected " << threads << " hardware threads";

    //Using half the threads for graphics only
    threads /= 2;
    qDebug() << "Scene Manager will have " << threads << "worker threads";

    qDebug() << "Init of Ogre for Qt5...\n";



    //Initialize Ogre Root
    root = make_unique<Ogre::Root>("plugin.cfg", "ogre.cfg", "ogre.log");

    Ogre::RenderSystem* renderSystem;

    //Register the HLMS
    switch(API)
    {
    case RenderAPI::OpenGL:
        qDebug() << "Rendering with OpenGL. Will use GLSL shaders";
        renderSystem = root->getRenderSystemByName(GL3PLUS_RENDERSYSTEM);
        shadingLanguage = "GLSL";
        break;
    case RenderAPI::DirectX11:
        qDebug() << "Rendering with DirectX11. Will use HLSL shaders";
        shadingLanguage = "HLSL";
        break;
    }

    root->setRenderSystem(renderSystem);
    renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");
    root->initialise(false);


    //Store the singleton address
    self = this;

}

std::tuple<Ogre::SceneManager *, Ogre::Camera *, Ogre::CompositorWorkspace *, Ogre::IdString> QtOgre21::WidgetCreatedCallback(Ogre::RenderWindow *virtualWindow)
{
    Ogre::SceneManager* smgr{nullptr};
    if(scenes.empty())
        scenes.push_back(smgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads, Ogre::INSTANCING_CULLING_THREADED));
    else
        smgr = scenes[0];

    Ogre::Camera* camera = smgr->createCamera("MyCamera" + std::to_string(cameraCounter++));

    auto compositorManager = root->getCompositorManager2();
    Ogre::IdString workspaceName { "MyWorkspace" + std::to_string(workspaceCounter++)};

    if(!compositorManager->hasWorkspaceDefinition(workspaceName))
        compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue(0.2,0.3,0.4));

    Ogre::CompositorWorkspace* workspace = compositorManager->addWorkspace(smgr, virtualWindow, camera, workspaceName, true);

    return std::tie<Ogre::SceneManager *, Ogre::Camera *, Ogre::CompositorWorkspace *, Ogre::IdString>(smgr, camera, workspace, workspaceName);

}

QtOgre21::~QtOgre21()
{
    //Forget the singleton address, because we're destructing
    self = nullptr;
}

QtOgre21* QtOgre21::instance()
{
    return self;
}
