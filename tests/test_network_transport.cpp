#include <QtTest>
#include "../src/core/NetworkTransport.h"

class TestNetworkTransport : public QObject {
    Q_OBJECT
private slots:
    void testConnect() {
        NetworkTransport transport;
        // In a unit test without a real server, connecting to localhost usually fails or hangs
        // So we might need a mock server or just test the API state changes if possible.
        // For MVP, checking API existence is a start.
        QSignalSpy spy(&transport, &NetworkTransport::connected);
        transport.connectToHost("127.0.0.1", 3240);
        // We expect it to try connecting.
        QCOMPARE(transport.state(), QAbstractSocket::ConnectingState);
    }
    
    void testPacketFormatting() {
        // Test static helper for creating USBIP header
        QByteArray header = NetworkTransport::createHeader(0x8005, 0); // OP_REQ_IMPORT
        QCOMPARE(header.size(), 8); // Minimum mock header size
    }
};

QTEST_MAIN(TestNetworkTransport)
#include "test_network_transport.moc"
