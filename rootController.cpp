#include "rootController.h"

#include <QDebug>

RootController::RootController(QObject *parent)
    : QObject(parent)
    , m_hub(std::make_unique<DevicesHub>())
    , m_type(Server)
{
    m_device = std::make_unique<Device>();
}

void RootController::findDevices()
{
    qDebug() << "find devices";
    m_hub->findDevices();
}

int RootController::deviceType()
{
    return m_type;
}
