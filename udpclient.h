#ifndef UPDCLIENT_H
#define UPDCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QLoggingCategory>

class UdpClient : public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpClient(const QString &host, const quint16 port, QUdpSocket *parent = nullptr);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

signals:
    void errorToGui(const QString &);
    void connectedToServer();

private:
    QLoggingCategory m_lc;
    QString m_host;
    quint16 m_port;
    QUdpSocket *m_UDPClient = nullptr;
};

#endif // UPDCLIENT_H
