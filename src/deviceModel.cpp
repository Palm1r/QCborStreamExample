#include "deviceModel.h"

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_deviceList.size();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const DeviceInfo &device = m_deviceList[index.row()];
    switch (role) {
    case IdRole:
        return device.id;
    case IpRole:
        return device.ip.toString();
    case MessageCountsRole:
        return device.messageCount;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[IdRole] = "deviceId";
    roles[IpRole] = "deviceIp";
    roles[MessageCountsRole] = "deviceMessageCount";

    return roles;
}

void DeviceModel::addDevice(DeviceInfo newDevice)
{
    for (const auto &device : qAsConst(m_deviceList)) {
        if (device.id == newDevice.id) {
            return;
        }
    }
    qDebug() << "add device" << newDevice.id << newDevice.ip << newDevice.messageCount;

    beginInsertRows(QModelIndex(), m_deviceList.size(), m_deviceList.size());
    m_deviceList.push_back({newDevice.id, newDevice.ip, newDevice.messageCount});
    endInsertRows();
}

void DeviceModel::addDeviceMessage(QString id)
{
    for (int i = 0; i < m_deviceList.size(); ++i) {
        if (m_deviceList.at(i).id == id) {
            m_deviceList[i].messageCount++;
            emit modelChanged();
            break;
        }
    }
}
