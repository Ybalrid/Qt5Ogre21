#ifndef QTOGRE21_H
#define QTOGRE21_H

#include <iostream>
#include <stdexcept>

class QtOgre21
{
public:
    QtOgre21();
    ~QtOgre21();
    static QtOgre21* instance();
private:
    static QtOgre21* self;
    bool rendersystemReady;

};

#endif // QTOGRE21_H
