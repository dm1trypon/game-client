#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QLoggingCategory>
#include <QTimer>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(const QString host, const quint16 port, QTcpSocket *parent = nullptr);

    void addToQueue(const QString &data);
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSend();
signals:
    void message(const QByteArray &);
    void errorToGui(const QString &);
    void connectedToServer();
private:
    QTimer *m_tSender = nullptr;
    QLoggingCategory m_lc;
    QString m_host;
    quint16 m_port;
    QByteArray m_recvBuf;
    QStringList m_dispatchQueue;
};

#endif // TCPCLIENT_H
