#ifndef DEVICEHUB_H
#define DEVICEHUB_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

#include "deviceModel.h"

namespace {
constexpr quint16 tcpListenPort = 56666;
constexpr quint16 broadcastPort = 45454;
} // namespace

class HubTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit HubTcpServer(QObject *parent = nullptr) { listen(QHostAddress::Any, tcpListenPort); };

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle) override { emit deviceConnected(handle); };

signals:
    void deviceConnected(qintptr newSocket);
};

class DevicesHub : public QObject
{
    Q_OBJECT
public:
    explicit DevicesHub(QObject *parent = nullptr);

    void findDevices();

    void sendBroadcastData(QByteArray data);

    const std::shared_ptr<DeviceModel> &deviceList() const;

signals:
    void regNewDevice(DeviceInfo info);
    void newMessageFrom(const QString &id);

public slots:
    void acceptConnection(qintptr newSocket);

private:
    std::unique_ptr<QUdpSocket> m_udpSocket;
    std::unique_ptr<HubTcpServer> m_hubTcpServer;
    std::shared_ptr<DeviceModel> m_deviceList;
};

#endif // DEVICEHUB_H
