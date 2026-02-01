#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "../core/UsbDeviceManager.h"
#include "../core/NetworkTransport.h"

class QTreeWidget;
class QLineEdit;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshDeviceList();
    void onConnectClicked();
    void onShareClicked(); // Context menu action

private:
    void setupUi();
    
    QTreeWidget *deviceList;
    QLineEdit *ipInput;
    QPushButton *connectButton;
    QLabel *statusLabel;
    
    UsbDeviceManager *deviceManager; // Polymorphic ownership
    NetworkTransport *transport;
};

#endif // MAINWINDOW_H
