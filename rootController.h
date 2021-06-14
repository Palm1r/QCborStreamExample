#ifndef ROOTCONTROLLER_H
#define ROOTCONTROLLER_H

#include <memory>
#include <QObject>

#include "device.h"
#include "devicesHub.h"

class RootController : public QObject
{
    Q_OBJECT
public:
    enum DeviceType { Server, Client };
    Q_ENUM(DeviceType)

    explicit RootController(QObject *parent = nullptr);

public slots:
    void findDevices();
    int deviceType();

signals:

private:
    std::unique_ptr<DevicesHub> m_hub;
    DeviceType m_type;
    std::unique_ptr<Device> m_device;
};

#endif // ROOTCONTROLLER_H
