#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>

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
};

#endif // SERVER_H
