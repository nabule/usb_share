#include "WindowsUsbDeviceManager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QFile>

QList<UsbDevice> WindowsUsbDeviceManager::enumerateDevices() {
    QList<UsbDevice> devices;
    
    QProcess process;
    process.start("usbipd", {"list"});
    if (!process.waitForFinished(5000)) {
        process.start("C:\\Program Files\\usbipd-win\\usbipd.exe", {"list"});
        if (!process.waitForFinished(5000)) return devices;
    }
    
    QByteArray rawOutput = process.readAllStandardOutput();
    // 优先尝试 UTF-8 (usbipd-win 4.x 默认)
    QString output = QString::fromUtf8(rawOutput);
    if (output.count(QChar::ReplacementCharacter) > 10 || output.trimmed().isEmpty()) {
        output = QString::fromLocal8Bit(rawOutput);
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    // VID:PID regex (e.g. 046d:c52f)
    QRegularExpression idRegex("([0-9a-fA-F]{4}):([0-9a-fA-F]{4})");

    for (const QString &line : lines) {
        QString l = line.trimmed();
        QRegularExpressionMatch idMatch = idRegex.match(l);
        
        if (idMatch.hasMatch()) {
            UsbDevice dev;
            QString fullId = idMatch.captured(0); // "046d:c52f"
            int idPos = l.indexOf(fullId);
            
            // 1. BUSID: VID:PID 左边的所有内容
            dev.busId = l.left(idPos).trimmed();
            if (dev.busId.isEmpty()) continue; // 忽略没有 BUSID 的行
            
            // 2. VID/PID
            dev.vendorId = idMatch.captured(1).toUShort(nullptr, 16);
            dev.productId = idMatch.captured(2).toUShort(nullptr, 16);
            
            // 3. 描述与状态: VID:PID 右边的所有内容 (ID 长度通常为 9)
            QString rest = l.mid(idPos + fullId.length()).trimmed();
            
            // 4. 提取状态: 检查结尾
            QString lowerRest = rest.toLower();
            if (lowerRest.endsWith("not shared")) {
                dev.isShared = false;
                dev.description = rest.left(rest.length() - 10).trimmed();
            } else if (lowerRest.endsWith("shared")) {
                dev.isShared = true;
                dev.description = rest.left(rest.length() - 6).trimmed();
            } else if (lowerRest.endsWith("attached")) {
                dev.isShared = true;
                dev.description = rest.left(rest.length() - 8).trimmed();
            } else {
                // 兜底方案：如果状态不是英文或格式不对
                dev.description = rest;
                dev.isShared = false;
            }
            
            devices.append(dev);
        }
    }

    // 更新调试日志，记录解析到了多少个设备
    QFile debugFile("usbipd_debug.txt");
    if (debugFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        debugFile.write(QString("\n--- PARSER SUMMARY ---\nParsed Devices: %1\n").arg(devices.size()).toUtf8());
        debugFile.close();
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