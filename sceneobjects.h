#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "consts.h"

#include <QObject>
#include <QMap>
#include <QList>

using namespace GameData;

class SceneObjects : public QObject
{
    Q_OBJECT
public:
    explicit SceneObjects(QObject *parent = nullptr);

    void setGameObjects(GameObjects gameObjects);
    GameObjects getGameObjects();
private:
    GameObjects m_gameObjects;
};

#endif // SCENEOBJECTS_H
