#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    process.start("usbipd", {"list"});
    if (!process.waitForFinished(5000)) {
        return devices;
    }
    
    QByteArray rawOutput = process.readAllStandardOutput();
    QString output = QString::fromLocal8Bit(rawOutput);
    if (output.trimmed().isEmpty()) {
        output = QString::fromUtf8(rawOutput);
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    // Improved Regex:
    // 1: BUSID (e.g., 1-1)
    // 2: VID (4 hex)
    // 3: PID (4 hex)
    // 4: The rest (Device + State)
    QRegularExpression lineRe("^(\\d+-\\d+)\\s+([0-9a-fA-F]{4}):([0-9a-fA-F]{4})\\s+(.+)$");

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        QRegularExpressionMatch match = lineRe.match(trimmed);
        
        if (match.hasMatch()) {
            UsbDevice dev;
            dev.busId = match.captured(1);
            dev.vendorId = match.captured(2).toUShort(nullptr, 16);
            dev.productId = match.captured(3).toUShort(nullptr, 16);
            
            QString rest = match.captured(4).trimmed();
            
            // State is at the end. Usually "Not shared", "Shared", or "Attached"
            // We search for common states at the end of the string
            if (rest.endsWith("Not shared", Qt::CaseInsensitive)) {
                dev.isShared = false;
                dev.description = rest.left(rest.length() - 10).trimmed();
            } else if (rest.endsWith("Shared", Qt::CaseInsensitive)) {
                dev.isShared = true;
                dev.description = rest.left(rest.length() - 6).trimmed();
            } else if (rest.endsWith("Attached", Qt::CaseInsensitive)) {
                dev.isShared = true;
                dev.description = rest.left(rest.length() - 8).trimmed();
            } else {
                // Fallback for Chinese or other states: assume the last "word" is the state
                int lastSpace = rest.lastIndexOf(QRegularExpression("\\s{2,}"));
                if (lastSpace > 0) {
                    dev.description = rest.left(lastSpace).trimmed();
                    QString state = rest.mid(lastSpace).trimmed().toLower();
                    dev.isShared = state.contains("shared") || state.contains("attached");
                } else {
                    dev.description = rest;
                    dev.isShared = false;
                }
            }
            
            devices.append(dev);
        }
    }
    
    return devices;
}

bool WindowsUsbDeviceManager::bindDevice(const QString& busId) {
    return executeUsbipCommand({"bind", "--busid", busId});
}

bool WindowsUsbDeviceManager::unbindDevice(const QString& busId) {
    return executeUsbipCommand({"unbind", "--busid", busId});
}

bool WindowsUsbDeviceManager::executeUsbipCommand(const QStringList& args) {
    QProcess process;
    process.start("usbipd", args);
    process.waitForFinished();
    return (process.exitCode() == 0);
}
