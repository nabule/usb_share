#include <QtTest>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include "../src/gui/MainWindow.h"

class TestGui : public QObject {
    Q_OBJECT
private slots:
    void testConnectButton() {
        MainWindow w;
        // Find "Connect" button and IP input
        QPushButton *btn = w.findChild<QPushButton*>("connectButton");
        QLineEdit *ipInput = w.findChild<QLineEdit*>("ipInput");
        
        QVERIFY(btn != nullptr);
        QVERIFY(ipInput != nullptr);
        
        // Simulate input
        QTest::keyClicks(ipInput, "192.168.1.100");
        QCOMPARE(ipInput->text(), "192.168.1.100");
        
        // Simulate click
        // QTest::mouseClick(btn, Qt::LeftButton);
        // Verify state change (would require mocking the backend)
    }
    void testDiscoveryUI() {
        MainWindow w;
        QTreeWidget *list = w.findChild<QTreeWidget*>("discoveryList");
        QVERIFY(list != nullptr);
        QCOMPARE(list->headerItem()->text(0), "Hostname");
    }
};

QTEST_MAIN(TestGui)
#include "test_gui.moc"
