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
    
    setupUi();
    
    resize(600, 400);
    setWindowTitle("USB Share (MVP)");
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
    
    statusLabel = new QLabel("Ready");
    remoteLayout->addWidget(statusLabel);
    remoteLayout->addStretch();
    
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
