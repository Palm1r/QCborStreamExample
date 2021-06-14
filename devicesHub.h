#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

class DevicesHub : public QObject
{
    Q_OBJECT
public:
    explicit DevicesHub(QObject *parent = nullptr);

    void findDevices();

    void sendBroadcastData(QByteArray data);

signals:

public slots:
    void acceptConnection();
    void readyToRead();

private:
    std::unique_ptr<QUdpSocket> m_udpSocket;
    std::unique_ptr<QTcpServer> m_hubTcpServer;
    QTcpSocket *tcpServerConnection = nullptr;
};

#endif // SERVER_H
