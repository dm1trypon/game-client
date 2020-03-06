#ifndef CONSTS_H
#define CONSTS_H

#include <QList>

namespace GameData {
    struct Scene {
        int x;
        int y;
        int width;
        int height;
    };

    struct Player {
        int x;
        int y;
        int width;
        int height;
    };

    struct Bullet {
        int x;
        int y;
        int width;
        int height;
    };

    struct GameObjects {
        QList<Player> players;
        QList<Bullet> bullets;
        QList<Scene> scene;
    };
}

namespace GameContent {
    const QMap<QString, QString> TEXTURES_PATHS {
        {"player", ":/textures/player.png"},
        {"scene", ":/textures/scene.png"},
        {"enemy_player", ":/textures/enemy-player.png"},
        {"bullet", ":/textures/bullet.png"}
    };
}

#endif // CONSTS_H
