#include "devicesHub.h"

#include <QCborStreamWriter>

namespace {
constexpr int broadcastPort = 45454;
}

DevicesHub::DevicesHub(QObject *parent)
    : QObject(parent)
    , m_udpSocket(std::make_unique<QUdpSocket>())
    , m_hubTcpServer(std::make_unique<HubTcpServer>())
    , m_deviceList(std::make_shared<DeviceModel>())
{
    connect(m_hubTcpServer.get(),
            &HubTcpServer::deviceConnected,
            this,
            &DevicesHub::acceptConnection);
}

void DevicesHub::findDevices()
{
    QByteArray data;
    {
        QCborStreamWriter writer(&data);
        writer.startMap(2);
        writer.append("type");
        writer.append("server");
        writer.append("command");
        writer.append("find");
        writer.endArray();
    }
    sendBroadcastData(data);
}

void DevicesHub::sendBroadcastData(QByteArray data)
{
    m_udpSocket->writeDatagram(data, QHostAddress::Broadcast, broadcastPort);
}

void DevicesHub::acceptConnection(qintptr newSocket)
{
    DeviceInfo device;
    auto in_socket = std::make_shared<QTcpSocket>();
    in_socket->setSocketDescriptor(newSocket);
    qDebug() << "ip:port" << in_socket->peerAddress() << ":" << in_socket->peerPort();
    device.ip = in_socket->peerAddress();

    connect(in_socket.get(), &QTcpSocket::readyRead, [in_socket]() {
        auto bytearray = in_socket->readAll();
        qDebug() << "get data" << bytearray;
        in_socket->close();
    });

    //    in_socket->close();
    //    qDebug() << "server accept connection";
    //    tcpServerConnection = m_hubTcpServer->nextPendingConnection();
    //    if (!tcpServerConnection) {
    //        qDebug("Error: got invalid pending connection!");
    //        return;
    //    }
    //    qDebug() << "ip:port" << tcpServerConnection->peerAddress() << ":"
    //             << tcpServerConnection->peerPort();

    //    connect(tcpServerConnection, &QIODevice::readyRead, this, &DevicesHub::readyToRead);
    //    //    connect(tcpServerConnection, &QAbstractSocket::errorOccurred, this, &Dialog::displayError);
    //    connect(tcpServerConnection,
    //            &QTcpSocket::disconnected,
    //            tcpServerConnection,
    //            &QTcpSocket::deleteLater);

    //    //    serverStatusLabel->setText(tr("Accepted connection"));
    //    m_hubTcpServer->close();
}

void DevicesHub::readyToRead() {}

const std::shared_ptr<DeviceModel> &DevicesHub::deviceList() const
{
    return m_deviceList;
}
