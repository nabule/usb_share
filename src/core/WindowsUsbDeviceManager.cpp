#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    process.start("usbipd", {"list"});
    if (!process.waitForFinished(5000)) {
        qDebug() << "CRITICAL: usbipd list timeout or failed to start.";
        return devices;
    }
    
    // Try Local8Bit first, then UTF-8
    QByteArray rawOutput = process.readAllStandardOutput();
    QString output = QString::fromLocal8Bit(rawOutput);
    if (output.trimmed().isEmpty()) {
        output = QString::fromUtf8(rawOutput);
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    // VID:PID regex (e.g. 046d:c52b)
    QRegularExpression idRegex("([0-9a-fA-F]{4}):([0-9a-fA-F]{4})");

    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        
        QRegularExpressionMatch idMatch = idRegex.match(trimmed);
        if (idMatch.hasMatch()) {
            // Found a potential device line
            QStringList parts = trimmed.split(QRegularExpression("\s+"), Qt::SkipEmptyParts);
            
            // Expected format: BUSID  VID:PID  DEVICE_NAME...  STATE
            if (parts.size() >= 3) {
                UsbDevice dev;
                dev.busId = parts[0];
                dev.vendorId = idMatch.captured(1).toUShort(nullptr, 16);
                dev.productId = idMatch.captured(2).toUShort(nullptr, 16);
                
                // State is usually at the very end
                QString state = parts.last().toLower();
                dev.isShared = (state.contains("shared") && !state.contains("not")) || state.contains("attached");
                
                // Description is everything between VID:PID and State
                int idPos = trimmed.indexOf(idMatch.captured(0));
                int statePos = trimmed.lastIndexOf(parts.last());
                if (statePos > idPos + 10) {
                    dev.description = trimmed.mid(idPos + 10, statePos - (idPos + 10)).trimmed();
                } else {
                    dev.description = "USB Device";
                }
                
                devices.append(dev);
            }
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