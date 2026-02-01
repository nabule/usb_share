#ifndef NETWORKTRANSPORT_H
#define NETWORKTRANSPORT_H

#include <QTcpSocket>
#include <QObject>

class NetworkTransport : public QObject {
    Q_OBJECT
public:
    explicit NetworkTransport(QObject *parent = nullptr);
    
    void connectToHost(const QString &address, quint16 port);
    void disconnectFromHost();
    void sendData(const QByteArray &data);
    
    QAbstractSocket::SocketState state() const;

    // Static helper to create protocol headers (Mock implementation of USBIP protocol)
    static QByteArray createHeader(quint16 command, quint32 seqNum);

signals:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void dataReceived(const QByteArray &data);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
};

#endif // NETWORKTRANSPORT_H
