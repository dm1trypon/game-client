#include "sceneobjects.h"

SceneObjects::SceneObjects(QObject *parent) :
    QObject(parent)
{

}

void SceneObjects::setGameObjects(GameObjects gameObjects) {
    m_gameObjects = gameObjects;
}

GameObjects SceneObjects::getGameObjects() {
    return m_gameObjects;
}
