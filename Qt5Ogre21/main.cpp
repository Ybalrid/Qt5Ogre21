#include "qogreviewport.h"

#include <QApplication>
#include <memory>

#include "somecustomwidget.h"

int main(int argc, char *argv[])
{
    //Initialize the Qt Application
    QApplication a(argc, argv);

    //Initialize the QtOgre system
    QtOgre21 qtOgre(QtOgre21::RenderAPI::OpenGL);

    //Create the widget
    SomeCustomWidget widget; widget.show();
    auto w = widget.w;
    auto w2 = widget.w2;

    //Resource locations
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem");

    //HighLevelMaterialSystem shader libraries
    qtOgre.declareHlmsLibrary();

    //Initialize the resources
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //Setup 1st scene (that is in 1st widget
    auto scene = qtOgre.getScene(0);
    auto suzanneItem = scene->createItem(qtOgre.loadFromV1Mesh("Suzanne.mesh"));
    auto suzanneNode = scene->getRootSceneNode()->createChildSceneNode();
    suzanneNode->attachObject(suzanneItem);
    auto sunlight = scene->createLight();
    auto sunNode = scene->getRootSceneNode()->createChildSceneNode();
    sunNode->attachObject(sunlight);
    sunlight->setType(Ogre::Light::LT_DIRECTIONAL);
    sunlight->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
    sunlight->setPowerScale(Ogre::Math::PI);
    auto Camera = w->getCamera();
    Camera->setPosition(0, 1, 3.5f);

    //Setup 2nd scene (2nd widget)
    auto scene2 = qtOgre.getScene(1);
    auto knotItem = scene2->createItem(qtOgre.loadFromV1Mesh("knot.mesh"));
    auto knotNode = scene2->getRootSceneNode()->createChildSceneNode();
    knotNode->attachObject(knotItem);
    knotNode->setScale(0.01f, 0.01f, 0.01f);
    auto sunlight2 = scene2->createLight();
    auto sunNode2 = scene2->getRootSceneNode()->createChildSceneNode();
    sunNode2->attachObject(sunlight2);
    sunlight2->setType(Ogre::Light::LT_DIRECTIONAL);
    sunlight2->setDirection(Ogre::Vector3(-1, -1, -0.5f).normalisedCopy());
    sunlight2->setPowerScale(Ogre::Math::PI);
    auto Camera2 = w2->getCamera();
    Camera2->setPosition(0, 3, 0);
    Camera2->setOrientation(Ogre::Quaternion(Ogre::Degree(-90),
                                             Ogre::Vector3::UNIT_X));

    return a.exec();
}
