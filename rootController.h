#ifndef ROOTCONTROLLER_H
#define ROOTCONTROLLER_H

#include <QObject>

class RootController : public QObject
{
    Q_OBJECT
public:
    explicit RootController(QObject *parent = nullptr);

signals:

};

#endif // ROOTCONTROLLER_H
