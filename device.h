#ifndef DEVICE_H
#define DEVICE_H

#include <QCborStreamReader>
#include <QObject>
#include <QUdpSocket>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);

    void handleStream(QCborStreamReader &reader);
private slots:
    void readBroadcastData();
signals:

private:
    std::unique_ptr<QUdpSocket> m_listenSocket;
    std::unique_ptr<QCborStreamReader> m_cborReader;
};

#endif // DEVICE_H
