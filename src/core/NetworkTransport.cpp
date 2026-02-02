#include "NetworkTransport.h"
#include <QDataStream>
#include <QDebug>

NetworkTransport::NetworkTransport(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    
    // Disable Nagle's algorithm for low latency (as per tech-stack.md)
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    
    connect(socket, &QTcpSocket::connected, this, [this]() {
        qDebug() << "[NetworkTransport] Connected to host";
        emit connected();
    });
    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        qDebug() << "[NetworkTransport] Disconnected from host";
        emit disconnected();
    });
    connect(socket, &QTcpSocket::readyRead, this, &NetworkTransport::onReadyRead);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        qDebug() << "[NetworkTransport] Socket error:" << error << socket->errorString();
        emit errorOccurred(error);
    });
#else
    connect(socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, [this](QAbstractSocket::SocketError error) {
        qDebug() << "[NetworkTransport] Socket error:" << error << socket->errorString();
        emit errorOccurred(error);
    });
#endif
}

void NetworkTransport::connectToHost(const QString &address, quint16 port) {
    qDebug() << "[NetworkTransport] Connecting to" << address << ":" << port;
    socket->connectToHost(address, port);
}

void NetworkTransport::disconnectFromHost() {
    qDebug() << "[NetworkTransport] Disconnecting...";
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
    qDebug() << "[NetworkTransport] Received" << data.size() << "bytes";
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
