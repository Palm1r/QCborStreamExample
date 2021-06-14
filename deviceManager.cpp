#include "deviceManager.h"

#include <QDebug>

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
    , m_device(new Device(this, Device::Server))
    , m_connectionManager(new ConnectionManager(this, m_device))
{}

void DeviceManager::startFindDevices()
{
    qDebug() << "start find device";
    QByteArray datagram = "Broadcast message ";

    m_connectionManager->sendBroadcast(datagram);
}
