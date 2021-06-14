#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

#include "device.h"

class CBORServer : public QTcpServer
{
public:
    explicit CBORServer(QObject *parent) { listen(QHostAddress::Any); };

signals:

    // QTcpServer interface
protected:
    //    void incomingConnection(qintptr handle) override{
    //        emit
    //    };
};

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent, Device *device);

    void startServer();

    void sendBroadcast(QByteArray datagram);

public slots:
    void readBroadcast();

signals:

private:
    Device *m_device;

    QTcpSocket tcpClient;

    QUdpSocket broadcastSocket;
    CBORServer *m_server;
};

#endif // CONNECTIONMANAGER_H
