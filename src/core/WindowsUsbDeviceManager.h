#ifndef WINDOWSUSBDEVICEMANAGER_H
#define WINDOWSUSBDEVICEMANAGER_H

#include "UsbDeviceManager.h"

class WindowsUsbDeviceManager : public UsbDeviceManager {
public:
    QList<UsbDevice> enumerateDevices() override;
    bool bindDevice(const QString& busId) override;
    bool unbindDevice(const QString& busId) override;

private:
    // Helper to call usbip.exe or driver ioctls
    bool executeUsbipCommand(const QStringList& args);
};

#endif // WINDOWSUSBDEVICEMANAGER_H
