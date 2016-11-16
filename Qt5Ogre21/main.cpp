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

    return a.exec();
}
