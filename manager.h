#ifndef MANAGER_H
#define MANAGER_H

#include "tcpclient.h"
#include "udpclient.h"
#include "core.h"

#include <QObject>
#include <QJsonArray>
#include <QLoggingCategory>

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);

    void onConnect(const QString &host, const quint16 udpPort, const quint16 tcpPort);
    void toVerifyTcp(const QString &nickname, const QStringList resolution);
    void toVerifyUdp(const QString &nickname);
    void toMousePlayer(const QPair<int, int> position, bool isClicked);
private:
    TcpClient *m_tcpClient = nullptr;
    UdpClient *m_udpClient = nullptr;
    Core *m_core = nullptr;

    QString m_nickname;
    QString m_host;
    quint16 m_udpPort;
    quint16 m_tcpPort;

    QMap<QString, QString> m_texPaths;

    QStringList m_sides;

    QLoggingCategory m_lc;
signals:
    void nextFrame(GameObjects);
    void error(const QString &);
    void connectedTcp();
    void connectedUdp();

public slots:
    void toKeyboard(const QString &side, bool isPressed);

    void onMouseClick(bool isClick);
    void onMouseMove(const QPoint pos);
private slots:
    void onTcpServerConnected();
    void onUdpServerConnected();
    void onError(const QString &err);
    void onTcpMessage(const QByteArray data);
    void onUdpMessage(const QString msg);
};

#endif // MANAGER_H
