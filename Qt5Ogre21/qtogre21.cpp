#include "qtogre21.h"

QtOgre21* QtOgre21::self(nullptr);

QtOgre21::QtOgre21() :
    rendersystemReady{false}
{
    if(self)
        throw std::runtime_error("Cannot instanciate 2 QtOgre21 objects. This is a singleton class.");

    std::cout << "Init of Ogre for Qt5...\n";

    //Initialize Ogre Root

    //Register the HLMS

    //Store the singleton address
    self = this;
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
