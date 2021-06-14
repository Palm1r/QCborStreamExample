#include "devicesHub.h"

#include <QCborStreamReader>
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

    connect(this, &DevicesHub::regNewDevice, m_deviceList.get(), &DeviceModel::addDevice);
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
    auto in_socket = std::make_shared<QTcpSocket>();
    in_socket->setSocketDescriptor(newSocket);
    qDebug() << "ip:port" << in_socket->peerAddress() << ":" << in_socket->peerPort();

    connect(in_socket.get(), &QTcpSocket::readyRead, this, [in_socket, this]() {
        DeviceInfo device;
        device.id = "test"; /*in_socket->peerName();*/
        device.ip = in_socket->peerAddress();
        device.messageCount++;

        auto bytearray = in_socket->readAll();
        qDebug() << "get data" << bytearray;
        in_socket->close();

        QCborStreamReader reader(bytearray);
        if (reader.lastError() != QCborError::NoError || !reader.isMap())
            return;
        if (!reader.isLengthKnown() || reader.length() != 3)
            return;

        QVariantMap map;
        reader.enterContainer();

        int i = 0;
        QString key;
        QString value;

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
