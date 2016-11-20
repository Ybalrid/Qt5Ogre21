#ifndef QOGREVIEWPORT_H
#define QOGREVIEWPORT_H

#include <QWidget>
#include <QPaintEvent>
#include "qtogre21.h"

class QOgreViewport : public QWidget
{
    Q_OBJECT

public:
    QOgreViewport(size_t SceneManagerIndex = 0, QWidget *parent = 0);

    QPaintEngine* paintEngine() const {return nullptr;}

    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;

    void changeBackgroundColor(Ogre::ColourValue c);

    Ogre::Camera* getCamera();
    Ogre::SceneManager* getSmgr();

    ~QOgreViewport();
private:
    static size_t windowNumber;
    size_t smgrIndex;

protected:
    Ogre::RenderWindow* Window;
    Ogre::Camera* Camera;
    Ogre::SceneManager* SceneManager;
    Ogre::CompositorWorkspace* Workspace;
    Ogre::IdString WorkspaceDefName;


};

#endif // QOGREVIEWPORT_H
