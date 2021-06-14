#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QUdpSocket>

class HubTcpServer : public QTcpServer
{
public:
    explicit HubTcpServer(QObject *parent = nullptr) { listen(QHostAddress::Any, 56666); };

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) override { qDebug() << "incoming"; };
};

class DevicesHub : public QObject
{
    Q_OBJECT
public:
    explicit DevicesHub(QObject *parent = nullptr);

    void findDevices();

    void sendBroadcastData(QByteArray data);

signals:

private:
    std::unique_ptr<QUdpSocket> m_udpSocket;
    std::unique_ptr<QTcpServer> m_hubTcpServer;
};

#endif // SERVER_H
