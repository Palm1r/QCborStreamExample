#ifndef DEVICE_H
#define DEVICE_H

#include <QHostAddress>
#include <QObject>

class ConnectionManager;
class Device : public QObject
{
    Q_OBJECT
public:
    enum DeviceType { Server, Client };
    Q_ENUM(DeviceType)

    explicit Device(QObject *parent = nullptr, DeviceType type = Client);

signals:

private:
    DeviceType m_type;
    int m_broadcastPort;
    int m_id;
    QHostAddress m_ip;

    //    ConnectionManager *m_connectionManager;
};

#endif // DEVICE_H
