#ifndef MANAGER_H
#define MANAGER_H

#include "tcpclient.h"
#include "udpclient.h"

#include <QObject>
#include <QLoggingCategory>

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);

    void onConnect(const QString &host, const quint16 udpPort, const quint16 tcpPort);
    void toVerifyTcp(const QString &nickname, const QStringList resolution);
    void toVerifyUdp(const QString &nickname);
private:
    TcpClient *m_tcpClient = nullptr;
    UdpClient *m_udpClient = nullptr;

    QString m_host;
    quint16 m_udpPort;
    quint16 m_tcpPort;

    QLoggingCategory m_lc;

signals:
    void error(const QString &);
    void connectedTcp();
    void connectedUdp();

private slots:
    void onTcpServerConnected();
    void onError(const QString &err);
    void onMessage(const QByteArray data);
    void onUdpServerConnected();
};

#endif // MANAGER_H
