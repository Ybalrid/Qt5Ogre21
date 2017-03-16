#include "qtogre21.h"

using namespace std;


QtOgre21* QtOgre21::self(nullptr);
size_t QtOgre21::cameraCounter(0);
size_t QtOgre21::workspaceCounter(0);

QtOgre21::QtOgre21(RenderAPI API, Ogre::String HlmsLibraryPath) :
    root{ nullptr },
    glContext{ 0 },
    hlmsPath{ HlmsLibraryPath },
    AALevel{ 8 },
    defaultBackgroundColor{0.2f, 0.4f, 0.6f}
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
    root = make_unique<Ogre::Root>("", "ogre.cfg", "ogre.log");



    Ogre::RenderSystem* renderSystem;

    //Register the HLMS
    switch(API)
    {
    case RenderAPI::OpenGL:
        qDebug() << "Rendering with OpenGL. Will use GLSL shaders";
#ifdef QT_DEBUG
        root->loadPlugin("RenderSystem_GL3Plus_d");
#else
        root->loadPlugin("RenderSystem_GL3Plus");
#endif
        renderSystem = root->getRenderSystemByName(GL3PLUS_RENDERSYSTEM);
        shadingLanguage = "GLSL";
        break;
    case RenderAPI::DirectX11:
        qDebug() << "Rendering with DirectX11. Will use HLSL shaders";
#ifdef QT_DEBUG
        root->loadPlugin("RenderSystem_DX11_d");
#else
        root->loadPlugin("RenderSystem_DX11");
#endif
        renderSystem = root->getRenderSystemByName(DIREXTX11_RENDERSYSTEM);
        shadingLanguage = "HLSL";
        break;
    }

    if(!renderSystem) throw std::runtime_error("Render System is NULLPTR");

    root->setRenderSystem(renderSystem);
    renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");
    root->initialise(false);

    //Store the singleton address
    self = this;
    usedAPI = API;
}

void QtOgre21::createNewScene()
{
    scenes.push_back(Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads, Ogre::INSTANCING_CULLING_THREADED));
}

QtOgre21::WidgetCreationCallbackTuple
QtOgre21::WidgetCreatedCallback(Ogre::RenderWindow *virtualWindow, size_t sceneIndex)
{
    Ogre::SceneManager* smgr{nullptr};
    if(scenes.empty())
    {
        scenes.push_back(smgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, threads, Ogre::INSTANCING_CULLING_THREADED));
        logToOgre("Just created a scene manager");
    }
    else
    {
        smgr = getScene(sceneIndex);
        logToOgre(std::string("Retreived the ") + std::to_string(sceneIndex) +"th scene manager");
    }

    Ogre::Camera* camera = smgr->createCamera("MyCamera" + std::to_string(cameraCounter++));

    auto compositorManager = root->getCompositorManager2();
    Ogre::String workspaceNameStr;
    Ogre::IdString workspaceName;
    if(customCompositor.empty())
    {
        workspaceNameStr = "MyWorkspace" + std::to_string(workspaceCounter);
        workspaceName = workspaceNameStr;
        if(!compositorManager->hasWorkspaceDefinition(workspaceName))
        compositorManager->createBasicWorkspaceDef(workspaceNameStr, defaultBackgroundColor); //here I set a background color. The thing I would like to change later.
    }
    else
    {
        workspaceNameStr = customCompositor;
        workspaceName = customCompositor;
    }

    auto workspace = compositorManager->addWorkspace(smgr, virtualWindow, camera, workspaceName, true, int(workspaceCounter++));

    //if(workspaceCounter == 0) declareHlmsLibrary(hlmsPath.c_str());

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

void QtOgre21::declareHlmsLibrary()
{
    Ogre::String hlmsFolder = hlmsPath;
    qDebug() << "Loading " << hlmsFolder.c_str() << " as HLMS";

    if(hlmsFolder.empty()) hlmsFolder =  "./";
    else if (hlmsFolder[hlmsFolder.size() - 1] != '/') hlmsFolder += "/";

    auto hlmsManager = Ogre::Root::getSingleton().getHlmsManager();

    //Define the shader library to use for HLMS
    auto library = Ogre::ArchiveVec();
    auto archiveLibrary = Ogre::ArchiveManager::getSingleton().load(hlmsFolder + "Hlms/Common/" + shadingLanguage, "FileSystem", true);
    library.push_back(archiveLibrary);

    //Define "unlit" and "PBS" (physics based shader) HLMS
    auto archiveUnlit = Ogre::ArchiveManager::getSingleton().load(hlmsFolder + "Hlms/Unlit/" + shadingLanguage, "FileSystem", true);
    auto archivePbs = Ogre::ArchiveManager::getSingleton().load(hlmsFolder + "Hlms/Pbs/" + shadingLanguage, "FileSystem", true);
    auto hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &library);
    auto hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &library);
    hlmsManager->registerHlms(hlmsUnlit);
    hlmsManager->registerHlms(hlmsPbs);
}

void QtOgre21::willCreateWindowHint()
{
    if(workspaceCounter == 0) return;
    //Here one window has allready been created
#ifdef _WIN32
    if(usedAPI == RenderAPI::OpenGL)
        glContext = wglGetCurrentContext();
    else
#endif
    glContext = nullptr;
}

HGLRC QtOgre21::getContext()
{
    return glContext;
}

QtOgre21::RenderAPI QtOgre21::getAPI()
{
    return usedAPI;
}

void QtOgre21::logToOgre(string message)
{
    Ogre::LogManager::getSingleton().logMessage(message);
}

void QtOgre21::setAALevel(uint8_t AA)
{
    AALevel = AA;
}

uint8_t QtOgre21::getAALevel()
{
    return AALevel;
}

void QtOgre21::setDefaultBackgroundColor(const Ogre::ColourValue& c)
{
    defaultBackgroundColor = c;
}

void QtOgre21::setCustomCompositor(const Ogre::String &name)
{
    customCompositor = name;
}
