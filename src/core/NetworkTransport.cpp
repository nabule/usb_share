#include "NetworkTransport.h"
#include <QDataStream>

NetworkTransport::NetworkTransport(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    
    // Disable Nagle's algorithm for low latency (as per tech-stack.md)
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    
    connect(socket, &QTcpSocket::connected, this, &NetworkTransport::connected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkTransport::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkTransport::onReadyRead);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkTransport::errorOccurred);
#else
    connect(socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &NetworkTransport::errorOccurred);
#endif
}

void NetworkTransport::connectToHost(const QString &address, quint16 port) {
    socket->connectToHost(address, port);
}

void NetworkTransport::disconnectFromHost() {
    socket->disconnectFromHost();
}

void NetworkTransport::sendData(const QByteArray &data) {
    socket->write(data);
}

QAbstractSocket::SocketState NetworkTransport::state() const {
    return socket->state();
}

void NetworkTransport::onReadyRead() {
    QByteArray data = socket->readAll();
    emit dataReceived(data);
}

QByteArray NetworkTransport::createHeader(quint16 command, quint32 seqNum) {
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian); // USBIP uses Big Endian
    
    // Simplified Header Structure for MVP
    stream << (quint16)0x0111; // Version 1.1.1 mock
    stream << command;
    stream << seqNum;
    
    return header;
}
