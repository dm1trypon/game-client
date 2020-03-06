#include "tcpclient.h"

#define TIME_SEND 10

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
            deleteLater();
        });
    m_tSender = new QTimer;
    connect(m_tSender, &QTimer::timeout, this, &TcpClient::onSend);
    connectToHost(host, port);
}

void TcpClient::addToQueue(const QString &data) {
    m_dispatchQueue.append(data);
}

void TcpClient::onSend() {
    if (m_dispatchQueue.isEmpty()) {
        return;
    }

    const QString &msg = m_dispatchQueue.first();
    qCInfo(m_lc) << QString("SENT: %1").arg(msg);

    if (write(msg.toUtf8()) < 0) {
        qCWarning(m_lc) << QString("Error sending data to server: %1").arg(msg);
        return;
    }

    m_dispatchQueue.removeFirst();
}

void TcpClient::onConnected() {
    qCInfo(m_lc) << "Connected";

    if (!m_tSender->isActive()) {
        m_tSender->start(TIME_SEND);
    }

    emit connectedToServer();
}

void TcpClient::onDisconnected() {
    qCInfo(m_lc) << "Disconnected";
}

void TcpClient::onReadyRead() {
    m_recvBuf += readAll();
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
