#include <QtTest>
#include <QTcpServer>
#include "../src/core/NetworkTransport.h"

class TestReproduceIssue : public QObject {
    Q_OBJECT

private slots:
    void testConnectionHang() {
        // 1. Setup Mock Server (simulating usbipd)
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHostIPv4, 0)); // Use dynamic port, force IPv4

        // 2. Setup Client (NetworkTransport)
        NetworkTransport transport;
        QSignalSpy spyConnected(&transport, &NetworkTransport::connected);
        QSignalSpy spyData(&transport, &NetworkTransport::dataReceived);

        // 3. Connect
        quint16 port = server.serverPort();
        qDebug() << "Client connecting to 127.0.0.1:" << port;
        transport.connectToHost("127.0.0.1", port);

        // 4. Wait for connection (Increased timeout for CI)
        if (!spyConnected.wait(5000)) {
            qDebug() << "Client failed to connect! Socket State:" << transport.state();
            // Check error
        } else {
             qDebug() << "Client Connected signal received.";
        }
        
        // 5. Verify TCP is connected
        QCOMPARE(transport.state(), QAbstractSocket::ConnectedState);

        // 6. Verify NO Handshake Data is sent by Client (Bug Confirmation)
        // If we were implementing the protocol, we should see data sent here.
        // But since we suspect we aren't sending anything, this proves the "hang" 
        // (Server waiting for data, Client sending nothing).
        
        // To verify what the server received:
        if (!server.waitForNewConnection(5000)) {
             qDebug() << "Server timeout waiting for connection. Server Error:" << server.serverError() << server.errorString();
             qDebug() << "Server listening on:" << server.serverAddress() << server.serverPort();
        }
        QVERIFY(server.waitForNewConnection(5000));
        QTcpSocket *serverSideSocket = server.nextPendingConnection();
        QVERIFY(serverSideSocket);
        
        // Wait a bit to see if client sends anything
        bool dataReceived = serverSideSocket->waitForReadyRead(500);
        QVERIFY(!dataReceived); // We EXPECT NO DATA, confirming we are just sitting there.
        
        serverSideSocket->close();
        server.close();
    }
};

QTEST_MAIN(TestReproduceIssue)
#include "test_reproduce_issue.moc"
