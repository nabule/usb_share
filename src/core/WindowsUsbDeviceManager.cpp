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
        qDebug() << "CRITICAL: Could not start 'usbipd'. Is it installed and in PATH?";
        return devices;
    }
    
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QString err = QString::fromLocal8Bit(process.readAllStandardError());
    
    if (!err.isEmpty()) {
        qDebug() << "usbipd list stderr:" << err;
    }

    // Logging to a temp file for user debugging
    QFile logFile("usbipd_output.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logFile.write(output.toUtf8());
        logFile.close();
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    bool headerFound = false;
    for (QString line : lines) {
        line = line.trimmed();
        if (line.isEmpty()) continue;

        // Look for the header to start capturing
        if (line.contains("BUSID") && line.contains("VID:PID")) {
            headerFound = true;
            continue;
        }

        if (headerFound) {
            // Stop if we hit a new section (like Persisted devices)
            if (line.contains("Persisted:") || line.contains("Currently shared:")) {
                // We keep going as we want all connected devices, 
                // but usually "Connected:" is the first section.
            }

            // More flexible Regex
            // 1: BUSID (e.g. 1-1 or 1-1.2)
            // 2: VID (4 hex)
            // 3: PID (4 hex)
            // 4: Description (anything in middle)
            // 5: State (at the end)
            QRegularExpression re("^(\\d+-\\d+(?:\\.\\d+)*)\\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})\\s+(.+?)\\s{2,}(.+)$");
            QRegularExpressionMatch match = re.match(line);
            
            if (match.hasMatch()) {
                UsbDevice dev;
                dev.busId = match.captured(1);
                dev.vendorId = match.captured(2).toUShort(nullptr, 16);
                dev.productId = match.captured(3).toUShort(nullptr, 16);
                dev.description = match.captured(4).trimmed();
                QString state = match.captured(5).trimmed().toLower();
                
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
