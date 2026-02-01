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
    
    // 优先尝试 UTF-8 编码 (usbipd-win 默认通常是 UTF-8)
    QString output = QString::fromUtf8(rawOutput);
    
    // 如果 UTF-8 解析出来的字符串包含太多乱码（替换字符），则尝试 Local8Bit
    if (output.count(QChar::ReplacementCharacter) > 10 || output.trimmed().isEmpty()) {
        output = QString::fromLocal8Bit(rawOutput);
    }

    // 调试日志更新：确保我们能看到解析后的真实文本
    QFile debugFile("usbipd_debug.txt");
    if (debugFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        debugFile.write("-- - DECODED OUTPUT ---\n");
        debugFile.write(output.toUtf8());
        debugFile.close();
    }

    QStringList lines = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    
    // 更加宽松的正则匹配
    // 1: BUSID (数字-数字)
    // 2: VID:PID (4位十六进制:4位十六进制)
    QRegularExpression lineRe("(\d+-\d+)\\s+([0-9a-fA-F]{4}:[0-9a-fA-F]{4})\\s+(.+)$");

    for (const QString &line : lines) {
        QString l = line.trimmed();
        QRegularExpressionMatch match = lineRe.match(l);
        
        if (match.hasMatch()) {
            UsbDevice dev;
            dev.busId = match.captured(1);
            
            QStringList idParts = match.captured(2).split(':');
            dev.vendorId = idParts[0].toUShort(nullptr, 16);
            dev.productId = idParts[1].toUShort(nullptr, 16);
            
            QString rest = match.captured(3).trimmed();
            
            // 简单的后端匹配：寻找常见的状态字符串
            if (rest.contains("Not shared", Qt::CaseInsensitive)) {
                dev.isShared = false;
                dev.description = rest.replace("Not shared", "", Qt::CaseInsensitive).trimmed();
            } else if (rest.contains("Shared", Qt::CaseInsensitive) || rest.contains("Attached", Qt::CaseInsensitive)) {
                dev.isShared = true;
                // 移除状态词以获取纯描述
                dev.description = rest.replace("Shared", "", Qt::CaseInsensitive)
                                      .replace("Attached", "", Qt::CaseInsensitive).trimmed();
            } else {
                // 如果是中文环境或其他状态，默认取前半部分
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
