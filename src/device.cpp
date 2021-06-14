#include "device.h"

#include <QCborStreamWriter>

Device::Device(QObject *parent)
    : QObject(parent)
    , m_listenSocket(std::make_unique<QUdpSocket>())
    , m_cborReader(std::make_unique<QCborStreamReader>())
{
    m_listenSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(m_listenSocket.get(), &QUdpSocket::readyRead, this, &Device::readBroadcastData);
    connect(&m_clientTcpSocket, &QAbstractSocket::connected, [this]() {
        qDebug() << "client connected";
        QByteArray answerData;
        {
            QCborStreamWriter writer(&answerData);
            writer.startMap(3);
            writer.append("type");
            writer.append("client");
            writer.append("id");
            writer.append("un1");
            writer.append("command");
            writer.append("reg");
            writer.endArray();
        }
        m_clientTcpSocket.write(answerData);
    });
}

void Device::readBroadcastData()
{
    QByteArray data;

    while (m_listenSocket->hasPendingDatagrams()) {
        data.resize(int(m_listenSocket->pendingDatagramSize()));
        if (m_listenSocket->readDatagram(data.data(), data.size(), &m_serverAdress, &m_serverPort)
            == -1)
            continue;

        m_cborReader->addData(data);

        if (m_cborReader->lastError() != QCborError::NoError || !m_cborReader->isMap())
            continue;
        if (!m_cborReader->isLengthKnown() || m_cborReader->length() != 2)
            continue;

        QVariantMap map;
        m_cborReader->enterContainer();

        int i = 0;
        QString key;
        QString value;

        while (m_cborReader->lastError() == QCborError::NoError && m_cborReader->hasNext()) {
            if (i == 0) {
                key = m_cborReader->readString().data;
                ++i;
            } else {
                value = m_cborReader->readString().data;
                map[key] = value;
                i = 0;
            }
            m_cborReader->next();
        }
        m_cborReader->leaveContainer();
        qDebug() << "map" << map << m_serverAdress << m_serverPort;
    }

    m_clientTcpSocket.connectToHost(m_serverAdress, 56666);
}
