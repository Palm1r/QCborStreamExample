#ifndef DEVICE_H
#define DEVICE_H

#include <QCborStreamReader>
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);

private slots:
    void readBroadcastData();
signals:

private:
    std::unique_ptr<QUdpSocket> m_listenSocket;
    std::unique_ptr<QCborStreamReader> m_cborReader;

    QHostAddress m_serverAdress;
    quint16 m_serverPort;
    QTcpSocket m_clientTcpSocket;
};

#endif // DEVICE_H
