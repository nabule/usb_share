#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>
#include <QList>
#include <QDateTime>

struct DiscoveredDevice {
    QString hostname;
    QString ip;
    quint16 port;
    QDateTime lastSeen;
    
    bool operator==(const DiscoveredDevice &other) const {
        return ip == other.ip && port == other.port;
    }
};

class UdpListener : public QObject {
    Q_OBJECT
public:
    explicit UdpListener(QObject *parent = nullptr);
    
    void start();
    void stop();
    
    QList<DiscoveredDevice> getDiscoveredDevices() const;
    
    // Public for testing
    bool processDatagram(const QByteArray &data, const QHostAddress &sender);

signals:
    void deviceDiscovered(const DiscoveredDevice &device);
    void deviceListUpdated();

private slots:
    void onReadyRead();
    void cleanUpStaleDevices();

private:
    QUdpSocket *udpSocket;
    QList<DiscoveredDevice> knownDevices;
    QTimer *cleanupTimer;
};

#endif // UDPLISTENER_H
