#ifndef USBDEVICEMANAGER_H
#define USBDEVICEMANAGER_H

#include <QString>
#include <QList>

struct UsbDevice {
    QString busId;      // e.g., "1-2"
    QString description;
    quint16 vendorId;
    quint16 productId;
    bool isShared;
};

class UsbDeviceManager {
public:
    virtual ~UsbDeviceManager() {}

    // Enumerate all local USB devices
    virtual QList<UsbDevice> enumerateDevices() = 0;

    // Bind a device to the stub driver to make it exportable
    virtual bool bindDevice(const QString& busId) = 0;

    // Unbind a device from the stub driver (return to host control)
    virtual bool unbindDevice(const QString& busId) = 0;
};

#endif // USBDEVICEMANAGER_H
