#include "devicesHub.h"

#include <memory>
#include <QCborStreamReader>
#include <QCborStreamWriter>

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
    connect(this, &DevicesHub::newMessageFrom, m_deviceList.get(), &DeviceModel::addDeviceMessage);
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
        writer.endMap();
    }
    sendBroadcastData(data);
}

void DevicesHub::sendBroadcastData(const QByteArray &data)
{
    m_udpSocket->writeDatagram(data, QHostAddress::Broadcast, broadcastPort);
}

void DevicesHub::acceptConnection(qintptr newSocket)
{
    auto in_socket = new QTcpSocket(this);
    in_socket->setSocketDescriptor(newSocket);

    auto remove_socket = [in_socket]() { in_socket->deleteLater(); };

    connect(in_socket, &QTcpSocket::errorOccurred, this, remove_socket);
    connect(in_socket, &QTcpSocket::disconnected, this, remove_socket);

    connect(in_socket, &QTcpSocket::readyRead, this, [in_socket, this]() {
        DeviceInfo device;
        device.ip = in_socket->peerAddress();

        QCborStreamReader reader(in_socket->readAll());

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
                if (key == "data") {
                    value = reader.toUnsignedInteger();
                } else {
                    value = reader.readString().data;
                }
                map[key] = value;
                i = 0;
            }
            reader.next();
        }
        qDebug() << "get map" << map;
        reader.leaveContainer();
        if (map["type"] == "client" && map["command"] == "reg") {
            device.id = map["id"].toString();
            emit regNewDevice(device);
        }
        if (map["type"] == "client" && map["command"] == "deviceData") {
            emit newMessageFrom(map["id"].toString());
        }
        in_socket->close();
    });
}

const std::shared_ptr<DeviceModel> &DevicesHub::deviceList() const
{
    return m_deviceList;
}
