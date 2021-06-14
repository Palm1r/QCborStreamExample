#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

#include "connectionManager.h"
#include "device.h"

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);

    Q_INVOKABLE void startFindDevices();

signals:

private:
    Device *m_device;
    ConnectionManager *m_connectionManager;
};

#endif // DEVICEMANAGER_H
