#include "tcpclient.h"

TcpClient::TcpClient(const QString host, const quint16 port, QTcpSocket *parent) :
    QTcpSocket(parent),
    m_lc("[TcpClient]"),
    m_host(host),
    m_port(port)
{
    qCInfo(m_lc) << QString("Connecting to TCP server %1:%2").arg(host).arg(QString::number(port));
    connect(this, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(this, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(this, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(this, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
        [=](QAbstractSocket::SocketError) {
            const QString &errMsg = QString("Error in connection to host: %1:%2").arg(host).arg(QString::number(port));
            qCWarning(m_lc) << errMsg;
            emit errorToGui(errMsg);
            this->deleteLater();
        });
    this->connectToHost(host, port);
}

void TcpClient::onConnected() {
    qCInfo(m_lc) << "Connected";
    emit connectedToServer();
}

void TcpClient::onDisconnected() {
    qCInfo(m_lc) << "Disconnected";
}

void TcpClient::onReadyRead() {
    m_recvBuf += this->readAll();
    int n;

    while ((n = m_recvBuf.indexOf('\n')) >= 0) {
        QByteArray msg = m_recvBuf.left(n);

        if (!msg.isEmpty()) {
            qCDebug(m_lc) << QString("RECD: %1").arg(QString::fromUtf8(msg));
            emit message(msg);
        }

        m_recvBuf.remove(0, n + 1);
    }
}
