#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);

signals:

};

#endif // DEVICEMANAGER_H
