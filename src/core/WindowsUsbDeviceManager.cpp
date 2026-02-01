#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    // In a real implementation, we would parse 'usbip.exe list -l' output.
    // Example output:
    //  - busid 1-1 (046d:c077)
    //    Logitech, Inc. : Mouse (046d:c077)
    
    QProcess process;
    process.start("usbip.exe", {"list", "-l"});
    process.waitForFinished();
    
    QString output = process.readAllStandardOutput();
    // Parsing logic would go here.
    // For MVP Phase 2, we just stub this out or allow Manual testing to verify.
    
    return devices;
}

bool WindowsUsbDeviceManager::bindDevice(const QString& busId) {
    return executeUsbipCommand({"bind", "-b", busId});
}

bool WindowsUsbDeviceManager::unbindDevice(const QString& busId) {
    return executeUsbipCommand({"unbind", "-b", busId});
}

bool WindowsUsbDeviceManager::executeUsbipCommand(const QStringList& args) {
    QProcess process;
    process.start("usbip.exe", args);
    process.waitForFinished();
    
    if (process.exitCode() != 0) {
        qDebug() << "usbip command failed:" << args << process.readAllStandardError();
        return false;
    }
    return true;
}
