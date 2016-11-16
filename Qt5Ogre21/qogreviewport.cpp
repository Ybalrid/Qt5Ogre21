#include "qogreviewport.h"

QOgreViewport::QOgreViewport(QWidget *parent)
    : QWidget(parent)
{
    if(!QtOgre21::instance())
        throw std::runtime_error("Cannot create QOgreViewport widgets if QtOgre21 is not initialized!");

}

QOgreViewport::~QOgreViewport()
{
}
