#ifndef QOGREVIEWPORT_H
#define QOGREVIEWPORT_H

#include <QWidget>
#include <QPaintEvent>
#include "qtogre21.h"

class QOgreViewport : public QWidget
{
    Q_OBJECT

public:
    ///Construct the viewport, give the index of the scene manager to use.
    /// If there's no created scene and the index is 0, will automatically create the 1st scene manager
    QOgreViewport(size_t SceneManagerIndex = 0, QWidget *parent = 0);

    ~QOgreViewport();

    ///Neutralize the paintEngine by returning nullptr
    QPaintEngine* paintEngine() const {return nullptr;}

    ///When the widget is repainted
    virtual void paintEvent(QPaintEvent *event) override;

    ///When the widget is resized
    virtual void resizeEvent(QResizeEvent *event) override;

    ///When the widget is moved
    virtual void moveEvent(QMoveEvent *event) override;

    void changeBackgroundColor(Ogre::ColourValue c);

    ///Return the camera used
    Ogre::Camera* getCamera();

    ///Return the SceneManager used
    Ogre::SceneManager* getSmgr();

private:
    ///Couner of windows
    static size_t windowNumber;

    ///Index of the scene manager used
    size_t smgrIndex;

protected:
    ///The RenderWindow that ogre knows
    Ogre::RenderWindow* Window;

    ///The Camera
    Ogre::Camera* Camera;

    ///The SMGR
    Ogre::SceneManager* SceneManager;

    ///The Compositor workspace used
    Ogre::CompositorWorkspace* Workspace;

    ///The name of the compositor workspace definition
    Ogre::IdString WorkspaceDefName;


};

#endif // QOGREVIEWPORT_H
