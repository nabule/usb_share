#include "MainWindow.h"
#include "../core/WindowsUsbDeviceManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    deviceManager = new WindowsUsbDeviceManager();
    transport = new NetworkTransport(this);
    udpListener = new UdpListener(this);
    udpBroadcaster = new UdpBroadcaster(this);
    
    setupUi();
    
    // Start services
    udpListener->start();
    udpBroadcaster->start(); // Auto-start broadcasting for MVP ease of use
    
    connect(udpListener, &UdpListener::deviceListUpdated, this, &MainWindow::updateDiscoveryList);
    
    resize(600, 500);
    setWindowTitle("USB Share (MVP + Discovery)");
}

MainWindow::~MainWindow() {
    delete deviceManager;
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QTabWidget *tabs = new QTabWidget();
    mainLayout->addWidget(tabs);
    
    // Tab 1: Local Devices
    QWidget *localTab = new QWidget();
    QVBoxLayout *localLayout = new QVBoxLayout(localTab);
    
    deviceList = new QTreeWidget();
    deviceList->setHeaderLabels({"Bus ID", "Description", "Status"});
    deviceList->setObjectName("deviceList");
    localLayout->addWidget(deviceList);
    
    QPushButton *refreshBtn = new QPushButton("Refresh");
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::refreshDeviceList);
    localLayout->addWidget(refreshBtn);
    
    tabs->addTab(localTab, "Local Devices");
    
    // Tab 2: Remote Connect
    QWidget *remoteTab = new QWidget();
    QVBoxLayout *remoteLayout = new QVBoxLayout(remoteTab);
    
    // Manual Connect Area
    QHBoxLayout *connLayout = new QHBoxLayout();
    ipInput = new QLineEdit();
    ipInput->setPlaceholderText("Remote IP Address");
    ipInput->setObjectName("ipInput");
    connLayout->addWidget(ipInput);
    
    connectButton = new QPushButton("Connect");
    connectButton->setObjectName("connectButton");
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connLayout->addWidget(connectButton);
    remoteLayout->addLayout(connLayout);
    
    // Discovery Area
    QLabel *discLabel = new QLabel("Discovered Devices:");
    remoteLayout->addWidget(discLabel);
    
    discoveryList = new QTreeWidget();
    discoveryList->setObjectName("discoveryList");
    discoveryList->setHeaderLabels({"Hostname", "IP Address", "Port", "Last Seen"});
    discoveryList->setIndentation(0);
    connect(discoveryList, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onDiscoveredDeviceClicked);
    remoteLayout->addWidget(discoveryList);
    
    statusLabel = new QLabel("Ready");
    remoteLayout->addWidget(statusLabel);
    
    tabs->addTab(remoteTab, "Remote Connect");
}

void MainWindow::refreshDeviceList() {
    deviceList->clear();
    auto devices = deviceManager->enumerateDevices();
    for (const auto &dev : devices) {
        QTreeWidgetItem *item = new QTreeWidgetItem(deviceList);
        item->setText(0, dev.busId);
        item->setText(1, dev.description);
        item->setText(2, dev.isShared ? "Shared" : "Local");
    }
}

void MainWindow::onConnectClicked() {
    QString ip = ipInput->text();
    if (ip.isEmpty()) return;
    
    statusLabel->setText("Connecting to " + ip + "...");
    transport->connectToHost(ip, 3240);
}

void MainWindow::onShareClicked() {
    // TODO: Implement sharing logic via context menu
}

void MainWindow::updateDiscoveryList() {
    discoveryList->clear();
    auto devices = udpListener->getDiscoveredDevices();
    for (const auto &dev : devices) {
        QTreeWidgetItem *item = new QTreeWidgetItem(discoveryList);
        item->setText(0, dev.hostname);
        item->setText(1, dev.ip);
        item->setText(2, QString::number(dev.port));
        item->setText(3, dev.lastSeen.toString("HH:mm:ss"));
        item->setData(0, Qt::UserRole, dev.ip); // Store IP in data
    }
}

void MainWindow::onDiscoveredDeviceClicked(QTreeWidgetItem *item, int column) {
    QString ip = item->data(0, Qt::UserRole).toString();
    ipInput->setText(ip);
    statusLabel->setText("Selected " + ip);
}