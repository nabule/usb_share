#ifndef USBIPWIN2CLIENT_H
#define USBIPWIN2CLIENT_H

#include <QObject>
#include <QString>
#include <QProcess>

class UsbIpWin2Client : public QObject {
    Q_OBJECT
public:
    explicit UsbIpWin2Client(QObject *parent = nullptr);

    void setExecutablePath(const QString &path);

    // usbip.exe attach -r <remote_ip> -b <bus_id>
    bool attach(const QString &remoteIp, const QString &busId);
    
    // usbip.exe detach -p <port>
    bool detach(int port);

    // usbip.exe list -r <remote_ip>
    QString list(const QString &remoteIp);

private:
    QString m_usbipPath;
    bool executeCommand(const QStringList &args, QString &output);
};

#endif // USBIPWIN2CLIENT_H
