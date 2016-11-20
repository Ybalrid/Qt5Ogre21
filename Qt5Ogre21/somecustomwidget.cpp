#include "somecustomwidget.h"

SomeCustomWidget::SomeCustomWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Some Window");
    setMinimumSize(100, 100);
    resize(1024,768);

    mainLayout = new QVBoxLayout();

    w = new QOgreViewport;
    QtOgre21::instance()->createNewScene();
    w2 = new QOgreViewport(1);

    mainLayout->addWidget(w);
    mainLayout->addWidget(w2);
    setLayout(mainLayout);
}
