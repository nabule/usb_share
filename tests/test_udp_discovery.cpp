#include <QtTest>
#include <QJsonDocument>
#include <QJsonObject>
#include "../src/core/UdpBroadcaster.h"
#include "../src/core/UdpListener.h"

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

    void testListenerParsing() {
        UdpListener listener;
        
        // Valid Packet
        QJsonObject obj;
        obj["cmd"] = "DISCOVERY_ANNOUNCE";
        obj["hostname"] = "SERVER-PC";
        obj["ip"] = "10.0.0.5";
        obj["port"] = 3240;
        QJsonDocument doc(obj);
        QByteArray data = doc.toJson();
        
        bool success = listener.processDatagram(data, QHostAddress("10.0.0.5"));
        QVERIFY(success);
        
        // Verify internal list updated
        auto devices = listener.getDiscoveredDevices();
        QCOMPARE(devices.size(), 1);
        QCOMPARE(devices[0].hostname, "SERVER-PC");
        
        // Duplicate Packet (should update timestamp, not add new)
        listener.processDatagram(data, QHostAddress("10.0.0.5"));
        QCOMPARE(listener.getDiscoveredDevices().size(), 1);
        
        // Invalid Packet
        success = listener.processDatagram("INVALID JSON", QHostAddress("10.0.0.6"));
        QVERIFY(!success);
    }
};

QTEST_MAIN(TestUdpDiscovery)
#include "test_udp_discovery.moc"
