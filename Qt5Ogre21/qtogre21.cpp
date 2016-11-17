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

Ogre::MeshPtr QtOgre21::loadFromV1Mesh(Ogre::String name)
{
    return loadFromV1Mesh(Ogre::v1::MeshManager::getSingleton().load
                          (name, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                           Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC ));
}

Ogre::MeshPtr QtOgre21::loadFromV1Mesh(Ogre::v1::MeshPtr v1Mesh)
{
    Ogre::MeshPtr v2Mesh;
    v2Mesh = Ogre::MeshManager::getSingletonPtr()->createManual(v1Mesh->getName() + "v2",
                                                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    v2Mesh->importV1(v1Mesh.get(), true, true, true);
    v1Mesh->unload();
    v1Mesh.setNull();

    return v2Mesh;
}

Ogre::SceneManager* QtOgre21::getScene(size_t index)
{
    if(index < scenes.size())
        return scenes[index];
    return nullptr;
}

void QtOgre21::declareHlmsLibrary(const Ogre::String&& path)
{
    Ogre::String hlmsFolder = path;

    if(hlmsFolder.empty()) hlmsFolder =  "./";
    else if (hlmsFolder[hlmsFolder.size() - 1] != '/') hlmsFolder += "/";

    auto hlmsManager = Ogre::Root::getSingleton().getHlmsManager();

    //Define the shader library to use for HLMS
    auto library = Ogre::ArchiveVec();
    auto archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(hlmsFolder + "Hlms/Common/" + shadingLanguage, "FileSystem", true);
    library.push_back(archiveLibrary);

    //Define "unlit" and "PBS" (physics based shader) HLMS
    auto archiveUnlit = Ogre::ArchiveManager::getSingletonPtr()->load(hlmsFolder + "Hlms/Unlit/" + shadingLanguage, "FileSystem", true);
    auto archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load(hlmsFolder + "Hlms/Pbs/" + shadingLanguage, "FileSystem", true);
    auto hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &library);
    auto hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &library);
    hlmsManager->registerHlms(hlmsUnlit);
    hlmsManager->registerHlms(hlmsPbs);

}
