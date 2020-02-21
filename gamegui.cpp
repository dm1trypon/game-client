#include "gamegui.h"

#include <math.h>
#include <QDebug>
#include <QBrush>
#include <QPen>

GameGUI::GameGUI(QWidget *parent) :
    QOpenGLWidget(parent),
    m_lc("[GameGUI]")
{

}

void GameGUI::paintEvent(QPaintEvent *event)
{
    foreach (Player player, m_gameObjects.players) {
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        paint(&painter, event, QPair<int, int>{player.x, player.y}, QPair<int, int>{player.width, player.height},
              TEXTURES_PATHS["player"]);

        painter.end();
        qCInfo(m_lc) << QString("Emited player's position: [%1:%2]").arg(QString::number(player.x)).arg(QString::number(player.y));
    }
}

void GameGUI::nextFrame(GameObjects gameObjects) {
    m_gameObjects = gameObjects;
    update();
}

void GameGUI::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    onKeyboardEvent(event, true);
}

void GameGUI::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        return;
    }

    onKeyboardEvent(event, false);
}

void GameGUI::onKeyboardEvent(QKeyEvent *event, bool isPressed) {
    QString side;

    switch(event->key()) {
    case Qt::Key_W:
        side = "up";
        break;
    case Qt::Key_S:
        side = "down";
        break;
    case Qt::Key_A:
        side = "left";
        break;
    case Qt::Key_D:
        side = "right";
        break;
    default:
        qCWarning(m_lc) << "Unallowed key";
        return;
    }

    emit keyboard(side, isPressed);
}



void GameGUI::paint(QPainter *painter, QPaintEvent *event, QPair<int, int> position, QPair<int, int> size,
                    const QString &texPath)
{
    QBrush background = QBrush(QColor(64, 32, 64));
    painter->fillRect(event->rect(), background);
//    painter->translate(100, 100);
    QPixmap pixmap2(texPath);
    painter->save();
    painter->drawPixmap(position.first, position.second, size.first, size.second, pixmap2);
    painter->restore();
}
