#include "connectionManager.h"

#include "device.h"
//#include <QCborStreamWriter>

ConnectionManager::ConnectionManager(QObject *parent, Device *device)
    : QObject(parent)
    , m_device(device)
{
    broadcastSocket.bind(50000, QUdpSocket::ShareAddress);
    connect(&broadcastSocket, &QUdpSocket::readyRead, this, &ConnectionManager::readBroadcast);
}

void ConnectionManager::startServer() {}

void ConnectionManager::sendBroadcast(QByteArray datagram)
{
    broadcastSocket.writeDatagram(datagram, QHostAddress::Broadcast, 50000);
}

void ConnectionManager::readBroadcast()
{
    qDebug() << "readBroadcast()";
    QByteArray datagram;
    while (broadcastSocket.hasPendingDatagrams()) {
        datagram.resize(int(broadcastSocket.pendingDatagramSize()));
        broadcastSocket.readDatagram(datagram.data(), datagram.size());
        qDebug() << "recieved" << datagram.constData();
    }
}
