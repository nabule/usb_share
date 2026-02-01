#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    // We use usbipd.exe as it is the recommended engine in README
    process.start("usbipd", {"list"});
    if (!process.waitForFinished()) {
        qDebug() << "Failed to start usbipd. Is it installed?";
        return devices;
    }
    
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    bool capturing = false;
    for (const QString &line : lines) {
        if (line.startsWith("BUSID")) {
            capturing = true;
            continue;
        }
        if (capturing && (line.trimmed().isEmpty() || line.contains("Persisted:"))) {
            capturing = false;
            break;
        }
        
        if (capturing) {
            // Regex to parse: BUSID  VID:PID  DEVICE  STATE
            // Example: 1-1    046d:c52b  Logitech USB Input Device  Not shared
            QRegularExpression re("^(\\S+)\\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})\\s+(.+?)\\s+(Shared|Not shared|Attached)$");
            QRegularExpressionMatch match = re.match(line.trimmed());
            if (match.hasMatch()) {
                UsbDevice dev;
                dev.busId = match.captured(1);
                dev.vendorId = match.captured(2).toUShort(nullptr, 16);
                dev.productId = match.captured(3).toUShort(nullptr, 16);
                dev.description = match.captured(4).trimmed();
                dev.isShared = (match.captured(5) == "Shared" || match.captured(5) == "Attached");
                devices.append(dev);
            }
        }
    }
    
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
