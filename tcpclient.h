#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QLoggingCategory>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(const QString host, const quint16 port, QTcpSocket *parent = nullptr);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
signals:
    void message(const QByteArray &);
    void errorToGui(const QString &);
    void connectedToServer();
private:
    QLoggingCategory m_lc;
    QString m_host;
    quint16 m_port;
    QByteArray m_recvBuf;
};

#endif // TCPCLIENT_H
