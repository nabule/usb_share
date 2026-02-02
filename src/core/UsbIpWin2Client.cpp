#include "UsbIpWin2Client.h"
#include <QDebug>
#include <QFileInfo>

UsbIpWin2Client::UsbIpWin2Client(QObject *parent) : QObject(parent) {
    // Default path assumption, can be overridden
    m_usbipPath = "usbip.exe"; 
}

void UsbIpWin2Client::setExecutablePath(const QString &path) {
    m_usbipPath = path;
}

bool UsbIpWin2Client::attach(const QString &remoteIp, const QString &busId) {
    // usbip.exe attach -r <remote_ip> -b <bus_id>
    QString output;
    QStringList args = {"attach", "-r", remoteIp, "-b", busId};
    if (executeCommand(args, output)) {
        // Check output for success message
        // "successfully attached to port X"
        return output.contains("successfully attached", Qt::CaseInsensitive);
    }
    return false;
}

bool UsbIpWin2Client::detach(int port) {
    // usbip.exe detach -p <port>
    QString output;
    QStringList args = {"detach", "-p", QString::number(port)};
    return executeCommand(args, output);
}

QString UsbIpWin2Client::list(const QString &remoteIp) {
    // usbip.exe list -r <remote_ip>
    QString output;
    QStringList args = {"list", "-r", remoteIp};
    executeCommand(args, output);
    return output;
}

bool UsbIpWin2Client::executeCommand(const QStringList &args, QString &output) {
    QProcess process;
    process.setProgram(m_usbipPath);
    process.setArguments(args);
    
    qDebug() << "[UsbIpWin2Client] Executing:" << m_usbipPath << args.join(" ");
    
    process.start();
    if (!process.waitForFinished(10000)) { // 10s timeout
        qWarning() << "[UsbIpWin2Client] Process timed out or failed to start";
        return false;
    }
    
    output = QString::fromUtf8(process.readAllStandardOutput());
    QString error = QString::fromUtf8(process.readAllStandardError());
    
    if (!error.isEmpty()) {
        qWarning() << "[UsbIpWin2Client] Stderr:" << error;
    }
    
    if (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0) {
        return true;
    }
    
    qWarning() << "[UsbIpWin2Client] Process failed with exit code:" << process.exitCode();
    return false;
}
