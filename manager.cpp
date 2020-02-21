#include "manager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QPair>

Manager::Manager(QObject *parent) :
    QObject(parent),
    m_lc("[Manager]")
{
    m_texPaths.insert("player", ":/textures/player.png");
    m_texPaths.insert("enemy_player", ":/textures/enemy-player.png");
    m_texPaths.insert("bullet", ":/textures/bullet.png");
}

void Manager::onTcpMessage(const QByteArray data) {
    const QJsonDocument dataJsDoc = QJsonDocument::fromJson(data);

    if (dataJsDoc.isNull()) {
        qCWarning(m_lc) << "Invalid json";
        return;
    }

    if (!dataJsDoc.isObject()) {
        qCWarning(m_lc) << "Is not an object";
        return;
    }

    const QJsonObject dataJsObj = dataJsDoc.object();

    if (!dataJsObj.contains("method")) {
        qCWarning(m_lc) << "Missing field 'method'";
        return;
    }

    const QString &method = dataJsObj.value("method").toString();

    if (method == "init_tcp") {
         m_udpClient = new UdpClient(m_host, m_udpPort);

         connect(m_udpClient, &UdpClient::errorToGui, this, &Manager::onError);
         connect(m_udpClient, &UdpClient::connectedToServer, this, &Manager::onUdpServerConnected);
         connect(m_udpClient, &UdpClient::message, this, &Manager::onUdpMessage);

         m_udpClient->connectToServer();
    }
}

void Manager::onUdpMessage(const QString msg) {
    const QJsonDocument dataJsDoc = QJsonDocument::fromJson(msg.toUtf8());

    if (dataJsDoc.isNull()) {
        qCWarning(m_lc) << "Invalid json";
        return;
    }

    if (!dataJsDoc.isObject()) {
        qCWarning(m_lc) << "Is not an object";
        return;
    }

    if (!m_core) {
        qCCritical(m_lc) << "Core is not init yet";
        return;
    }

    m_core->onNextMessage(dataJsDoc.object());
}

void Manager::toMousePlayer(const QPair<int, int> position, bool isClicked) {
    const QJsonObject keyboardJsObj {
        {"method", "mouse"},
        {"nickname", m_nickname},
        {"position", QJsonObject {
                {"x", position.first},
                {"y", position.second}
            }
        },
        {"is_clicked", isClicked}
    };

    const QJsonDocument keyboardJsDoc(keyboardJsObj);
    const QString data(keyboardJsDoc.toJson(QJsonDocument::Compact) + "\n");

    qCInfo(m_lc) << QString("TCP SENT: %1").arg(data);

    m_tcpClient->write(data.toUtf8());
}

void Manager::toKeyboard(const QString &side, bool isPressed) {
    if (isPressed && !m_sides.contains(side)) {
        m_sides.append(side);
    } else if (m_sides.contains(side) && !isPressed) {
        while (m_sides.removeOne(side)) {}
    }

    QJsonArray keysArr;

    if (!m_sides.isEmpty()) {
        keysArr = QJsonArray::fromStringList(m_sides);
    }

    qDebug() << keysArr << m_sides;

    const QJsonObject keyboardJsObj {
        {"method", "keyboard"},
        {"nickname", m_nickname},
        {"keys", keysArr}
    };

    const QJsonDocument keyboardJsDoc(keyboardJsObj);
    const QString data(keyboardJsDoc.toJson(QJsonDocument::Compact) + "\n");

    qCInfo(m_lc) << QString("TCP SENT: %1").arg(data);

    m_tcpClient->write(data.toUtf8());
}

void Manager::toVerifyUdp(const QString &nickname) {
    const QJsonObject verifyJsObj {
        {"method", "init_udp"},
        {"nickname", nickname}
    };

    const QJsonDocument verifyJsDoc(verifyJsObj);
    const QString data(verifyJsDoc.toJson(QJsonDocument::Compact) + "\n");

    qCInfo(m_lc) << QString("UDP SENT: %1").arg(data);

//    m_udpClient->write(data.toUtf8());
    m_udpClient->writeDatagram(data.toUtf8(), QHostAddress(m_host), m_udpPort);
    qCInfo(m_lc) << "after point";
}

void Manager::toVerifyTcp(const QString &nickname, const QStringList resolution) {
    m_nickname = nickname;

    const QJsonObject verifyJsObj {
        {"method", "init_tcp"},
        {"nickname", nickname},
        {"resolution", QJsonObject {
                {"width", resolution.first()},
                {"height", resolution.last()}
            }
        }
    };

    const QJsonDocument verifyJsDoc(verifyJsObj);
    const QString data(verifyJsDoc.toJson(QJsonDocument::Compact) + "\n");

    qCInfo(m_lc) << QString("TCP SENT: %1").arg(data);

    m_tcpClient->write(data.toUtf8());
}

void Manager::onConnect(const QString &host, const quint16 udpPort, const quint16 tcpPort) {
    m_udpPort = udpPort;
    m_tcpPort = tcpPort;
    m_host = host;

    m_tcpClient = new TcpClient(host, tcpPort);

    connect(m_tcpClient, &TcpClient::connectedToServer, this, &Manager::onTcpServerConnected);
    connect(m_tcpClient, &TcpClient::message, this, &Manager::onTcpMessage);
    connect(m_tcpClient, &TcpClient::errorToGui, this, &Manager::onError);
}

void Manager::onError(const QString &err) {
    emit error(err);
}

void Manager::onUdpServerConnected() {
    m_core = new Core;
    connect(m_core, &Core::nextFrame, this, &Manager::nextFrame);
    emit connectedUdp();
}

void Manager::onTcpServerConnected() {
    emit connectedTcp();
}

