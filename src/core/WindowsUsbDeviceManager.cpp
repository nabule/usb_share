#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QFile>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    process.start("usbipd", {"list"});
    if (!process.waitForFinished()) {
        qDebug() << "CRITICAL: Could not start 'usbipd'.";
        return devices;
    }
    
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    bool headerFound = false;
    for (QString line : lines) {
        line = line.trimmed();
        if (line.isEmpty()) continue;

        if (line.contains("BUSID") && line.contains("VID:PID")) {
            headerFound = true;
            continue;
        }

        if (headerFound) {
            // If we hit a line that doesn't look like a device (no VID:PID colon), skip
            if (!line.contains(":")) {
                if (line.contains("Persisted:") || line.contains("Currently shared:")) continue;
                // If it's a completely different section, we could break, but let's be safe
                continue;
            }

            // Simple split by multiple spaces
            QStringList parts = line.split(QRegularExpression("\\s{2,}"), Qt::SkipEmptyParts);
            
            // Expected parts: BUSID, VID:PID, DEVICE, STATE
            // Sometimes DEVICE name has spaces, but split(\\s{2,}) should keep it together
            if (parts.size() >= 4) {
                UsbDevice dev;
                dev.busId = parts[0].trimmed();
                
                QStringList idParts = parts[1].split(":");
                if (idParts.size() == 2) {
                    dev.vendorId = idParts[0].toUShort(nullptr, 16);
                    dev.productId = idParts[1].toUShort(nullptr, 16);
                }
                
                dev.description = parts[2].trimmed();
                QString state = parts[3].trimmed().toLower();
                dev.isShared = (state.contains("shared") && !state.contains("not shared")) || 
                                state.contains("attached");
                
                devices.append(dev);
            }
        }
    }
    
    qDebug() << "Enumerated" << devices.size() << "devices.";
    return devices;
}

bool WindowsUsbDeviceManager::bindDevice(const QString& busId) {
    // usbipd-win uses 'bind' to share a device
    return executeUsbipCommand({"bind", "--busid", busId});
}

bool WindowsUsbDeviceManager::unbindDevice(const QString& busId) {
    // usbipd-win uses 'unbind' to stop sharing
    return executeUsbipCommand({"unbind", "--busid", busId});
}

bool WindowsUsbDeviceManager::executeUsbipCommand(const QStringList& args) {
    QProcess process;
    process.start("usbipd", args);
    process.waitForFinished();
    
    if (process.exitCode() != 0) {
        qDebug() << "usbipd command failed:" << args << process.readAllStandardError();
        return false;
    }
    return true;
}
