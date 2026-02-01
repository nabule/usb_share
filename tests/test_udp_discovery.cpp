#include <QtTest>
#include <QJsonDocument>
#include <QJsonObject>
#include "../src/core/UdpBroadcaster.h"

class TestUdpDiscovery : public QObject {
    Q_OBJECT
private slots:
    void testBroadcastPacketFormat() {
        UdpBroadcaster broadcaster;
        // Mock data
        QString hostname = "TEST-PC";
        QString ip = "192.168.1.100";
        quint16 port = 3240;
        
        QByteArray packet = broadcaster.createPacket(hostname, ip, port);
        
        // Verify JSON validity
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(packet, &error);
        QCOMPARE(error.error, QJsonParseError::NoError);
        
        // Verify Content
        QJsonObject obj = doc.object();
        QCOMPARE(obj["cmd"].toString(), "DISCOVERY_ANNOUNCE");
        QCOMPARE(obj["hostname"].toString(), hostname);
        QCOMPARE(obj["ip"].toString(), ip);
        QCOMPARE(obj["port"].toInt(), (int)port);
    }
};

QTEST_MAIN(TestUdpDiscovery)
#include "test_udp_discovery.moc"
