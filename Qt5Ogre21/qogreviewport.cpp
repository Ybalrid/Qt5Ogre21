#include "qogreviewport.h"

size_t QOgreViewport::windowNumber{0};

QOgreViewport::QOgreViewport(QWidget *parent)
    : QWidget(parent),
      Window{nullptr}
{
    if(!QtOgre21::instance())
        throw std::runtime_error("Cannot create QOgreViewport widgets if QtOgre21 is not initialized!");

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);
    setWindowTitle(QString("Window") + QString(std::to_string(windowNumber).c_str()));
    Ogre::NameValuePairList misc;
    misc["vsync"] = "false";
    misc["externalWindowHandle"] = std::to_string(winId());

    Window = Ogre::Root::getSingleton().createRenderWindow(std::string("Window") + std::to_string(windowNumber++), width(), height(), false, &misc);

    auto result = QtOgre21::instance()->WidgetCreatedCallback(Window);

    SceneManager = std::get<0>(result);
    Camera = std::get<1>(result);
    Ogre::CompositorWorkspace* workspace = std::get<2>(result);

    Camera->setNearClipDistance(0.1);
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
    qDebug() << "paint event";
    Ogre::Root::getSingleton().renderOneFrame();
    event->accept();
}

void QOgreViewport::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(event->isAccepted())
    {
        const QSize& size = event->size();
        //Window->resize(size.width(), size.height());
        Window->windowMovedOrResized();
        this->update();
    }
}

void QOgreViewport::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    if(event->isAccepted())
    {
        Window->windowMovedOrResized();
        this->update();
    }
}

Ogre::Camera* QOgreViewport::getCamera()
{
    return Camera;
}
