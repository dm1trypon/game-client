#include "udpclient.h"

#include <QDebug>

UdpClient::UdpClient(const QString &host, const quint16 port, QUdpSocket *parent) :
    QUdpSocket(parent),
    m_lc("[UdpClient]"),
    m_host(host),
    m_port(port)
{
    qCInfo(m_lc) << QString("Connecting to UDP server %1:%2").arg(host).arg(QString::number(port));
    connect(this, &QUdpSocket::connected, this, &UdpClient::onConnected);
    connect(this, &QUdpSocket::disconnected, this, &UdpClient::onDisconnected);
    connect(this, &QUdpSocket::readyRead, this, &UdpClient::onReadyRead);
    connect(this, static_cast<void (QUdpSocket::*)(QAbstractSocket::SocketError)>(&QUdpSocket::error),
        [=](QAbstractSocket::SocketError) {
            const QString &errMsg = QString("Error in connection to host: %1:%2").arg(host).arg(QString::number(port));
            qCWarning(m_lc) << errMsg;
            emit errorToGui(errMsg);
            this->deleteLater();
        });
    this->connectToHost(host, port);
}

void UdpClient::onConnected() {
    qCInfo(m_lc) << "Connected";
    emit connectedToServer();
    qCInfo(m_lc) << "Emited";
}

void UdpClient::onDisconnected() {
    qCInfo(m_lc) << "Disconnected";
}

void UdpClient::onReadyRead() {
    qDebug() << "onReadyRead";
    while (this->hasPendingDatagrams()) {
        QByteArray datagram;
        const int theSize = this->pendingDatagramSize();
        datagram.resize(theSize);
        this->readDatagram(datagram.data(), datagram.size());
        QString data((const QByteArray&) datagram);
        qCInfo(m_lc) << QString("Recieved data: %1").arg(data);
    }
}
