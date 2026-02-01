#ifndef UDPBROADCASTER_H
#define UDPBROADCASTER_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QTimer>

class UdpBroadcaster : public QObject {
    Q_OBJECT
public:
    explicit UdpBroadcaster(QObject *parent = nullptr);
    
    void start(quint16 port = 3240); // Service port to announce
    void stop();
    
    // Public for testing
    QByteArray createPacket(const QString &hostname, const QString &ip, quint16 port);

private slots:
    void sendBroadcast();

private:
    QUdpSocket *udpSocket;
    QTimer *timer;
    quint16 servicePort;
};

#endif // UDPBROADCASTER_H
