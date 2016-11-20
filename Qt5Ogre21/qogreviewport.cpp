#include "qogreviewport.h"

size_t QOgreViewport::windowNumber{0};

QOgreViewport::QOgreViewport(size_t SceneManagerIndex, QWidget *parent)
    : QWidget(parent),
      smgrIndex{SceneManagerIndex},
      Window{nullptr}
{
    if(!QtOgre21::instance())
        throw std::runtime_error("Cannot create QOgreViewport widgets if QtOgre21 is not initialized!");

    //Permit Ogre to draw manually on that window
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);

    //Set the title of that window
    setWindowTitle(QString("Window") + QString(std::to_string(windowNumber).c_str()));

    //Confirugation for Ogre
    Ogre::NameValuePairList misc;
    misc["FSAA"] = std::to_string(QtOgre21::instance()->getAALevel());
    misc["vsync"] = "false";
    misc["externalWindowHandle"] = std::to_string(winId());
    QtOgre21::instance()->willCreateWindowHint();
    if(QtOgre21::instance()->getAPI() == QtOgre21::RenderAPI::OpenGL)
        if(auto context = QtOgre21::instance()->getContext())
            misc["externalGLContext"] = std::to_string(size_t(context));

    //Create the RenderWindow
    Window = Ogre::Root::getSingleton().createRenderWindow(std::string("Window") + std::to_string(windowNumber++), width(), height(), false, &misc);

    //We have to do this callback. This initialize the scene and the compositor for the rendering, and return a bunch of pointers to usefull stuff as a tuple
    auto result = QtOgre21::instance()->WidgetCreatedCallback(Window, SceneManagerIndex);

    //Extract the "bunch of usefull pointers"
    SceneManager = std::get<0>(result);
    Camera = std::get<1>(result);
    Workspace = std::get<2>(result);
    WorkspaceDefName = std::get<3>(result);

    //Configure the camera to a state where it will be able to draw something...
    Camera->setNearClipDistance(0.1f);
    Camera->setFarClipDistance(500);
    Camera->setPosition(0,0,10);
    Camera->lookAt(0,0,0);
    Camera->setAutoAspectRatio(true);


}

QOgreViewport::~QOgreViewport()
{

}

void QOgreViewport::paintEvent(QPaintEvent *event)
{
    //Call the refresh of Ogre when the viewport's window is called
    //qDebug() << "paint event";
    Ogre::Root::getSingleton().renderOneFrame();
    Window->windowMovedOrResized();

    event->accept();
}

//Tell Ogre the window has been resized and redraw
void QOgreViewport::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(event->isAccepted())
    {
        Window->windowMovedOrResized();
        this->update();
    }
}

//Same as above, but for movement.
void QOgreViewport::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    if(event->isAccepted())
    {
        Window->windowMovedOrResized();
        this->update();
    }
}

//Return the camera of this viewport
Ogre::Camera* QOgreViewport::getCamera()
{
    return Camera;
}

Ogre::SceneManager* QOgreViewport::getSmgr()
{
    return SceneManager;
}

void QOgreViewport::changeBackgroundColor(Ogre::ColourValue c)
{
}
