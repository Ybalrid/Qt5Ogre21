#include "qogreviewport.h"

QOgreViewport::QOgreViewport(QWidget *parent)
    : QWidget(parent),
      Window{nullptr}
{
    if(!QtOgre21::instance())
        throw std::runtime_error("Cannot create QOgreViewport widgets if QtOgre21 is not initialized!");

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);

    Ogre::NameValuePairList misc;
    misc["vsync"] = "false";
    misc["externalWindowHandle"] = std::to_string(winId());

    Window = Ogre::Root::getSingleton().createRenderWindow("some name here", width(), height(), false, &misc);

    auto result = QtOgre21::instance()->WidgetCreatedCallback(Window);
    SceneManager = std::get<0>(result);
    Camera = std::get<1>(result);

    Camera->setPosition(0,0,10);

}

QOgreViewport::~QOgreViewport()
{

}

void QOgreViewport::paintEvent(QPaintEvent *event)
{
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
