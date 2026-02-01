#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "../core/UsbDeviceManager.h"
#include "../core/NetworkTransport.h"
#include "../core/UdpListener.h"
#include "../core/UdpBroadcaster.h"

class QTreeWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QTreeWidgetItem;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshDeviceList();
    void onConnectClicked();
    void onShareClicked(); // Context menu action
    void onDiscoveredDeviceClicked(QTreeWidgetItem *item, int column);
    void updateDiscoveryList();

private:
    void setupUi();
    
    QTreeWidget *deviceList;
    QLineEdit *ipInput;
    QPushButton *connectButton;
    QLabel *statusLabel;
    
    // Remote Tab Discovery UI
    QTreeWidget *discoveryList;
    
    UsbDeviceManager *deviceManager; // Polymorphic ownership
    NetworkTransport *transport;
    UdpListener *udpListener;
    UdpBroadcaster *udpBroadcaster;
};

#endif // MAINWINDOW_H
