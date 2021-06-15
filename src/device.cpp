#include "device.h"

#include <QCborStreamWriter>
#include <QDate>

#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>

namespace {
constexpr int udpSocketPort = 45454;

void fillRegData(const QString &id, QByteArray &data)
{
    QCborStreamWriter writer(&data);
    writer.startMap(3);
    writer.append("type");
    writer.append("client");
    writer.append("id");
    writer.append(id);
    writer.append("command");
    writer.append("reg");
    writer.endMap();
}
void fillDeviceData(const QString &id, QByteArray &data)
{
    QCborStreamWriter writer(&data);
    writer.startMap(4);
    writer.append("type");
    writer.append("client");
    writer.append("id");
    writer.append(id);
    writer.append("command");
    writer.append("deviceData");
    writer.append("data");
    writer.append(QDate::currentDate().day());
    writer.endMap();
}
} // namespace

Device::Device(QObject *parent)
    : QObject(parent)
    , m_listenSocket(std::make_unique<QUdpSocket>())
    , m_clientTcpSocket(std::make_unique<QTcpSocket>())
{
    m_listenSocket->bind(udpSocketPort, QUdpSocket::ShareAddress);
    connect(m_listenSocket.get(), &QUdpSocket::readyRead, this, &Device::readBroadcastData);
    connect(m_clientTcpSocket.get(), &QAbstractSocket::connected, this, [this]() {
        QByteArray data;
        if (m_deviceState == DeviceState::Registration) {
            fillRegData(m_id, data);
            m_deviceState = DeviceState::SendingData;
        } else if (m_deviceState == DeviceState::SendingData) {
            fillDeviceData(m_id, data);
        }
        m_clientTcpSocket->write(data);
        if (!m_deviceDataSender.isActive()) {
            m_deviceDataSender.start();
        }
    });

    m_deviceDataSender.setInterval(3000);
    connect(&m_deviceDataSender, &QTimer::timeout, this, &Device::sendDeviceData);
}

void Device::readBroadcastData()
{
    QByteArray data;
    quint16 serverPort;

    while (m_listenSocket->hasPendingDatagrams()) {
        data.resize(int(m_listenSocket->pendingDatagramSize()));
        if (m_listenSocket->readDatagram(data.data(), data.size(), &m_serverAdress, &serverPort)
            == -1)
            continue;

        m_cborReader = std::make_unique<QCborStreamReader>(data);

        if (m_cborReader->lastError() != QCborError::NoError || !m_cborReader->isMap()) {
            qWarning("datastream no valid");
            continue;
        }
        if (!m_cborReader->isLengthKnown() || m_cborReader->length() != 3) {
            qWarning("datastream length unknown");
            continue;
        }

        QVariantMap map;
        m_cborReader->enterContainer();

        int i = 0;
        QString key;
        QVariant value;

        while (m_cborReader->lastError() == QCborError::NoError && m_cborReader->hasNext()) {
            if (i == 0) {
                key = m_cborReader->readString().data;
                ++i;
            } else {
                if (key == "listenport") {
                    value = m_cborReader->toUnsignedInteger();
                } else {
                    value = m_cborReader->readString().data;
                }
                map[key] = value;
                i = 0;
            }
            m_cborReader->next();
        }
        m_cborReader->leaveContainer();

        if (map["type"] == "server" && map["command"] == "find") {
            m_deviceState = DeviceState::Registration;
            m_listenServerPort = map["listenport"].toUInt();
        }
    }

    m_clientTcpSocket->connectToHost(m_serverAdress, m_listenServerPort);
}

void Device::sendDeviceData()
{
    m_clientTcpSocket->connectToHost(m_serverAdress, m_listenServerPort);
}

void Device::setId(const QString &newId)
{
    m_id = newId;
}
