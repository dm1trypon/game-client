#include "core.h"

#define TMR_FPS 17

Core::Core(QObject *parent) :
    QObject(parent),
    m_lc("[Core]")
{
    m_tmrNextFrame = new QTimer;
//    connect(m_tmrNextFrame, &QTimer::timeout, this, &Core::onNextFrame);
    m_tmrNextFrame->start(TMR_FPS);
}

void Core::onNextFrame() {
    emit nextFrame(m_sceneObjects.getGameObjects());
}

void Core::onNextMessage(const QJsonObject gameDataObj) {
    if (!gameDataObj.contains("players")) {
        qCWarning(m_lc) << "Wrong Game Protocol: 'players' is not found";
        return;
    }

    if (!gameDataObj.value("players").isArray()) {
        qCWarning(m_lc) << "Wrong Game Protocol: 'players' is not an array";
        return;
    }

    const QJsonArray playersArr = gameDataObj.value("players").toArray();
    const QList<Player> playersList = makeObjectsList(playersArr, "players", Player{});
    const QJsonArray bulletsArr = gameDataObj.value("bullets").toArray();
    const QList<Bullet> bulletsList = makeObjectsList(bulletsArr, "bullets", Bullet{});

    QJsonArray sceneArr;
    sceneArr.append(gameDataObj.value("scene").toObject());

    const QList<Scene> sceneList = makeObjectsList(sceneArr, "scene", Scene{});

    GameObjects gameObjects = {
        .players = playersList,
        .bullets = bulletsList,
        .scene = sceneList
    };

    emit nextFrame(m_sceneObjects.getGameObjects());
    m_sceneObjects.setGameObjects(gameObjects);
}

template<typename TypeObject>
QList<TypeObject> Core::makeObjectsList(const QJsonArray goArr, const QString &goName, TypeObject) {
    QList<TypeObject> goList;

    foreach (const QJsonValue goJsVal, goArr) {
        if (!goJsVal.isObject()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1' is not an object").arg(goName);
            return goList;
        }

        const QJsonObject goJsObj = goJsVal.toObject();

        // block player.position

        if (!goJsObj.contains("position")) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position' is not exist").arg(goName);
            return goList;
        }

        if (!goJsObj.value("position").isObject()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position' is not an object").arg(goName);
            return goList;
        }

        const QJsonObject goPosJsObj = goJsObj.value("position").toObject();

        if (!goPosJsObj.contains("x")) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position.x' is not exist").arg(goName);
            return goList;
        }

        if (!goPosJsObj.value("x").isDouble()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position.x' is not a double").arg(goName);
            return goList;
        }

        if (!goPosJsObj.contains("y")) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position.y' is not exist").arg(goName);
            return goList;
        }

        if (!goPosJsObj.value("y").isDouble()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.position.y' is not a double").arg(goName);
            return goList;
        }

        // block player.size

        if (!goJsObj.contains("size")) {
            qCWarning(m_lc) << "Wrong Game Protocol: '%1.size' is not exist";
            return goList;
        }

        if (!goJsObj.value("size").isObject()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.size' is not an object").arg(goName);
            return goList;
        }

        const QJsonObject goSizeJsObj = goJsObj.value("size").toObject();

        if (!goSizeJsObj.contains("width")) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.size.width' is not exist").arg(goName);
            return goList;
        }

        if (!goSizeJsObj.value("width").isDouble()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.size.width' is not a double").arg(goName);
            return goList;
        }

        if (!goSizeJsObj.contains("height")) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.size.height' is not exist").arg(goName);
            return goList;
        }

        if (!goSizeJsObj.value("height").isDouble()) {
            qCWarning(m_lc) << QString("Wrong Game Protocol: '%1.size.height' is not a double").arg(goName);
            return goList;
        }

        TypeObject gameObject = {
            .x = static_cast<int>(goPosJsObj.value("x").toDouble()),
            .y = static_cast<int>(goPosJsObj.value("y").toDouble()),
            .width = static_cast<int>(goSizeJsObj.value("width").toDouble()),
            .height = static_cast<int>(goSizeJsObj.value("height").toDouble())
        };

        goList.append(gameObject);
    }

    return goList;
}
