#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include <QCborStreamReader>
#include <QHostAddress>
#include <QTimer>

class QTcpSocket;
class QTimer;
class QUdpSocket;

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);

    enum class DeviceState {
        Listen,
        Registration,
        SendingData
    } m_deviceState
        = DeviceState::Listen;

    void setId(const QString &newId);

private slots:
    void readBroadcastData();
    void sendDeviceData();

private:
    QString m_id;
    std::unique_ptr<QUdpSocket> m_listenSocket;
    std::unique_ptr<QCborStreamReader> m_cborReader;
    std::unique_ptr<QTcpSocket> m_clientTcpSocket;

    QHostAddress m_serverAdress;
    uint m_listenServerPort;

    QTimer m_deviceDataSender;
};

#endif // DEVICE_H
