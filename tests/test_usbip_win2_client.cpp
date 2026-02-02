#include <QtTest>
#include "../src/core/UsbIpWin2Client.h"

class TestUsbIpWin2Client : public QObject {
    Q_OBJECT
private slots:
    void testInstantiation() {
        UsbIpWin2Client client;
        client.setExecutablePath("echo"); // Use echo to simulate success?
        // But echo won't return "successfully attached"
    }

    void testAttachFail() {
        UsbIpWin2Client client;
        client.setExecutablePath("nonexistent_usbip.exe");
        bool result = client.attach("127.0.0.1", "1-1");
        QVERIFY(result == false);
    }
};

QTEST_MAIN(TestUsbIpWin2Client)
#include "test_usbip_win2_client.moc"
