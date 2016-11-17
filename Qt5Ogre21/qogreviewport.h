#ifndef QOGREVIEWPORT_H
#define QOGREVIEWPORT_H

#include <QWidget>
#include <QPaintEvent>
#include "qtogre21.h"

class QOgreViewport : public QWidget
{
    Q_OBJECT

public:
    QOgreViewport(QWidget *parent = 0);

    QPaintEngine* paintEngine() const {return nullptr;}

    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;

    Ogre::Camera* getCamera();

    ~QOgreViewport();

private:
    Ogre::RenderWindow* Window;
    Ogre::Camera* Camera;
    Ogre::SceneManager* SceneManager;

    static size_t windowNumber;
};

#endif // QOGREVIEWPORT_H
