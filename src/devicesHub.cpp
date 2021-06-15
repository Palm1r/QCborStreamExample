#include "devicesHub.h"

#include <QCborStreamReader>
#include <QCborStreamWriter>

namespace {
constexpr quint16 broadcastPort = 45454;
constexpr quint16 tcpListenPort = 56666;
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

    connect(this, &DevicesHub::regNewDevice, m_deviceList.get(), &DeviceModel::addDevice);
}

void DevicesHub::findDevices()
{
    QByteArray data;
    {
        QCborStreamWriter writer(&data);
        writer.startMap(3);
        writer.append("type");
        writer.append("server");
        writer.append("command");
        writer.append("find");
        writer.append("listenport");
        writer.append(tcpListenPort);
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
    auto in_socket = std::make_shared<QTcpSocket>();
    in_socket->setSocketDescriptor(newSocket);
//    qDebug() << "ip:port" << in_socket->peerAddress() << ":" << in_socket->peerPort();

    connect(in_socket.get(), &QTcpSocket::readyRead, this, [in_socket, this]() {
        DeviceInfo device;
//        device.id = "test"; /*in_socket->peerName();*/
        device.ip = in_socket->peerAddress();
//        device.messageCount++;

        QCborStreamReader reader(in_socket->readAll());
        in_socket->close();

        if (reader.lastError() != QCborError::NoError || !reader.isMap())
            return;
        if (!reader.isLengthKnown())
            return;

        QVariantMap map;
        reader.enterContainer();

        int i = 0;
        QString key;
        QVariant value;

        while (reader.lastError() == QCborError::NoError && reader.hasNext()) {
            if (i == 0) {
                key = reader.readString().data;
                ++i;
            } else {
                value = reader.readString().data;
                map[key] = value;
                i = 0;
            }
            reader.next();
        }
        qDebug() << "get map" << map;
        reader.leaveContainer();
        device.id = map["id"].toString();
        emit regNewDevice(device);
    });
}

void DevicesHub::readyToRead() {}

const std::shared_ptr<DeviceModel> &DevicesHub::deviceList() const
{
    return m_deviceList;
}
