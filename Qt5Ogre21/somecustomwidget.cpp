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

    mainLayout->addWidget(w, 0);
    mainLayout->addWidget(w2, 0);

    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
}
