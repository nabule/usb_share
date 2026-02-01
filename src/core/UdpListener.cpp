#include "UdpListener.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

UdpListener::UdpListener(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);
    cleanupTimer = new QTimer(this);
    
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpListener::onReadyRead);
    connect(cleanupTimer, &QTimer::timeout, this, &UdpListener::cleanUpStaleDevices);
}

void UdpListener::start() {
    udpSocket->bind(3241, QUdpSocket::ShareAddress);
    cleanupTimer->start(5000); // Check for stale devices every 5s
}

void UdpListener::stop() {
    udpSocket->close();
    cleanupTimer->stop();
}

QList<DiscoveredDevice> UdpListener::getDiscoveredDevices() const {
    return knownDevices;
}

void UdpListener::onReadyRead() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        processDatagram(datagram.data(), datagram.senderAddress());
    }
}

bool UdpListener::processDatagram(const QByteArray &data, const QHostAddress &sender) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) return false;
    if (!doc.isObject()) return false;
    
    QJsonObject obj = doc.object();
    if (obj["cmd"].toString() != "DISCOVERY_ANNOUNCE") return false;
    
    DiscoveredDevice device;
    device.hostname = obj["hostname"].toString();
    // Prefer IP from packet, fallback to sender address
    device.ip = obj["ip"].toString();
    if (device.ip.isEmpty()) device.ip = sender.toString();
    
    device.port = (quint16)obj["port"].toInt();
    device.lastSeen = QDateTime::currentDateTime();
    
    // Update or Add
    bool found = false;
    for (int i = 0; i < knownDevices.size(); ++i) {
        if (knownDevices[i] == device) {
            knownDevices[i].lastSeen = device.lastSeen;
            knownDevices[i].hostname = device.hostname; // Update hostname if changed
            found = true;
            break;
        }
    }
    
    if (!found) {
        knownDevices.append(device);
        emit deviceDiscovered(device);
    }
    
    emit deviceListUpdated();
    return true;
}

void UdpListener::cleanUpStaleDevices() {
    QDateTime now = QDateTime::currentDateTime();
    QMutableListIterator<DiscoveredDevice> i(knownDevices);
    while (i.hasNext()) {
        if (i.next().lastSeen.secsTo(now) > 10) { // 10s timeout
            i.remove();
            emit deviceListUpdated();
        }
    }
}
