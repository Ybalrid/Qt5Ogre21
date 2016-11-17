#include "qogreviewport.h"

#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //U <3 RAII
    auto qtOgre { std::make_unique<QtOgre21>(QtOgre21::RenderAPI::OpenGL) };

    QOgreViewport w;
    w.show();

    QOgreViewport w2;
    w2.show();

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem");

    qtOgre->declareHlmsLibrary("HLMS");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto scene = qtOgre->getScene(0);
    auto suzanneItem = scene->createItem(qtOgre->loadFromV1Mesh("Suzanne.mesh"));
    auto suzanneNode = scene->getRootSceneNode()->createChildSceneNode();
    suzanneNode->attachObject(suzanneItem);

    auto sunlight = scene->createLight();
    auto sunNode = scene->getRootSceneNode()->createChildSceneNode();
    sunNode->attachObject(sunlight);
    sunlight->setType(Ogre::Light::LT_DIRECTIONAL);
    sunlight->setDirection(Ogre::Vector3(-1, -1, -0.5).normalisedCopy());
    sunlight->setPowerScale(Ogre::Math::PI);

//    auto Camera2 = w2.getCamera();
//    Camera2->setPosition(0,10,0);
//    Camera2->lookAt(0,0,0);

    return a.exec();
}
