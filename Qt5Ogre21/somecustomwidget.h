#ifndef SOMECUSTOMWIDGET_H
#define SOMECUSTOMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "qogreviewport.h"

class SomeCustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SomeCustomWidget(QWidget *parent = 0);

    QOgreViewport *w, *w2;

signals:

public slots:
private:
    QVBoxLayout* mainLayout;
};

#endif // SOMECUSTOMWIDGET_H
