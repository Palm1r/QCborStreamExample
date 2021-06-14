#include "device.h"

Device::Device(QObject *parent, DeviceType type)
    : QObject(parent)
    , m_type(type)
{}
