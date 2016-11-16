#ifndef QOGREVIEWPORT_H
#define QOGREVIEWPORT_H

#include <QWidget>
#include "qtogre21.h"

class QOgreViewport : public QWidget
{
    Q_OBJECT

public:
    QOgreViewport(QWidget *parent = 0);
    ~QOgreViewport();
};

#endif // QOGREVIEWPORT_H
