#include "deviceManager.h"

#include <QDebug>

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{}

//void DeviceManager::startFindDevices()
//{
//    //    qDebug() << "start find device";
//    //    QByteArray datagram = "Broadcast message ";

//    //    m_connectionManager->sendBroadcast(datagram);
//}
