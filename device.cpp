#include "device.h"

Device::Device(QObject *parent)
    : QObject(parent)
    , m_listenSocket(std::make_unique<QUdpSocket>())
    , m_cborReader(std::make_unique<QCborStreamReader>())
{
    m_listenSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(m_listenSocket.get(), &QUdpSocket::readyRead, this, &Device::readBroadcastData);
}

void Device::readBroadcastData()
{
    //    qDebug() << "readBroadcast()";
    QByteArray data;
    QHostAddress senderIp;
    quint16 senderPort;

    while (m_listenSocket->hasPendingDatagrams()) {
        data.resize(int(m_listenSocket->pendingDatagramSize()));
        if (m_listenSocket->readDatagram(data.data(), data.size(), &senderIp, &senderPort) == -1)
            continue;

        //        qDebug() << "recieved" << data.constData() << senderIp << senderPort;

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
        qDebug() << "map" << map;
    }
}
