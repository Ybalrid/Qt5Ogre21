#include "qogreviewport.h"

#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    new QtOgre21();

    QOgreViewport w;
    w.show();

    return a.exec();
}
