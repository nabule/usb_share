#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QFile>
#include <QStandardPaths>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    // Try to call usbipd. We assume it's in PATH or standard location.
    process.start("usbipd", {"list"});
    if (!process.waitForFinished(5000)) {
        // Fallback: try absolute path if not in PATH
        process.start("C:\\Program Files\\usbipd-win\\usbipd.exe", {"list"});
        if (!process.waitForFinished(5000)) {
            qDebug() << "CRITICAL: usbipd not found.";
            return devices;
        }
    }
    
    QByteArray rawOutput = process.readAllStandardOutput();
    // Use Local8Bit for Windows Console output (likely GBK on Chinese Windows)
    QString output = QString::fromLocal8Bit(rawOutput);
    if (output.trimmed().isEmpty()) {
        output = QString::fromUtf8(rawOutput); // Fallback to UTF-8
    }

    // DEBUG: Write output to file to help user diagnose
    QFile debugFile("usbipd_debug.txt");
    if (debugFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        debugFile.write("-- - RAW OUTPUT START ---\
");
        debugFile.write(rawOutput);
        debugFile.write("\n--- DECODED OUTPUT START ---\
");
        debugFile.write(output.toUtf8());
        debugFile.close();
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    // Robust parsing:
    // We look for lines containing a "xxxx:xxxx" pattern
    QRegularExpression idRegex("([0-9a-fA-F]{4}):([0-9a-fA-F]{4})");

    for (const QString &line : lines) {
        QString l = line.trimmed();
        QRegularExpressionMatch idMatch = idRegex.match(l);
        
        if (idMatch.hasMatch()) {
            int idPos = l.indexOf(idMatch.captured(0));
            if (idPos <= 0) continue;

            UsbDevice dev;
            // 1. Bus ID is everything before the VID:PID
            dev.busId = l.left(idPos).trimmed();
            
            // 2. VID/PID
            dev.vendorId = idMatch.captured(1).toUShort(nullptr, 16);
            dev.productId = idMatch.captured(2).toUShort(nullptr, 16);
            
            // 3. Everything after VID:PID (9 chars) is "Description + State"
            QString rest = l.mid(idPos + 9).trimmed();
            
            // 4. Split Description and State. State is usually at the end after multiple spaces.
            // We search for the last occurrence of 2 or more spaces.
            int stateStart = rest.lastIndexOf(QRegularExpression("\\s{2,}"));
            if (stateStart != -1) {
                dev.description = rest.left(stateStart).trimmed();
                QString stateStr = rest.mid(stateStart).trimmed().toLower();
                dev.isShared = stateStr.contains("shared") || stateStr.contains("attached");
            } else {
                // Fallback: just take the whole thing as description
                dev.description = rest;
                dev.isShared = false;
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
    if (!process.waitForFinished()) {
        process.start("C:\\Program Files\\usbipd-win\\usbipd.exe", args);
        process.waitForFinished();
    }
    return (process.exitCode() == 0);
}