#include "UdpBroadcaster.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostInfo>
#include <QNetworkInterface>

UdpBroadcaster::UdpBroadcaster(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    
    connect(timer, &QTimer::timeout, this, &UdpBroadcaster::sendBroadcast);
}

void UdpBroadcaster::start(quint16 port) {
    servicePort = port;
    timer->start(3000); // Broadcast every 3 seconds
}

void UdpBroadcaster::stop() {
    timer->stop();
}

void UdpBroadcaster::sendBroadcast() {
    QString hostname = QHostInfo::localHostName();
    
    // Find first non-loopback IPv4 address
    QString ip = "127.0.0.1";
    const QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (const QHostAddress &addr : list) {
        if (addr != QHostAddress::LocalHost && addr.toIPv4Address()) {
            ip = addr.toString();
            break;
        }
    }
    
    QByteArray packet = createPacket(hostname, ip, servicePort);
    udpSocket->writeDatagram(packet, QHostAddress::Broadcast, 3241); // Port 3241 for discovery
}

QByteArray UdpBroadcaster::createPacket(const QString &hostname, const QString &ip, quint16 port) {
    QJsonObject obj;
    obj["cmd"] = "DISCOVERY_ANNOUNCE";
    obj["hostname"] = hostname;
    obj["ip"] = ip;
    obj["port"] = (int)port;
    obj["version"] = "1.0";
    
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}
