#include <QtTest>
#include "../src/core/UsbDeviceManager.h"

// Mock implementation for testing
class MockUsbDeviceManager : public UsbDeviceManager {
public:
    QList<UsbDevice> enumerateDevices() override {
        return {
            {"1-1", "Mock Mouse", 0x1234, 0x5678, false},
            {"1-2", "Mock Keyboard", 0x8765, 0x4321, true}
        };
    }
    
    bool bindDevice(const QString& busId) override {
        if (busId == "1-1") return true;
        return false;
    }
    
    bool unbindDevice(const QString& busId) override {
        return true;
    }
};

class TestUsbExport : public QObject {
    Q_OBJECT
private slots:
    void testEnumerate() {
        MockUsbDeviceManager manager;
        auto devices = manager.enumerateDevices();
        QCOMPARE(devices.size(), 2);
        QCOMPARE(devices[0].busId, "1-1");
        QCOMPARE(devices[0].description, "Mock Mouse");
    }

    void testBind() {
        MockUsbDeviceManager manager;
        QVERIFY(manager.bindDevice("1-1"));
        QVERIFY(!manager.bindDevice("invalid-id"));
    }
};

QTEST_MAIN(TestUsbExport)
#include "test_usb_export.moc"
