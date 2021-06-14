#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractListModel>
#include <QHostAddress>

struct DeviceInfo
{
    QString id;
    QHostAddress ip;
    int messageCount = 0;
};

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles { IdRole = Qt::UserRole + 1, IpRole, MessageCountsRole };

    explicit DeviceModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addDevice(DeviceInfo newDevice);
signals:

private:
    QList<DeviceInfo> m_deviceList;
};

#endif // DEVICEMODEL_H
