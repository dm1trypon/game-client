#include "gamegui.h"

#include <math.h>
#include <QDebug>
#include <QBrush>
#include <QPen>

GameGUI::GameGUI(QWidget *parent) :
    QOpenGLWidget(parent),
    m_pmTexScene(QPixmap(TEXTURES_PATHS["scene"])),
    m_pmTexPlayer(QPixmap(TEXTURES_PATHS["player"])),
    m_lc("[GameGUI]")
{
    m_bufPosition = QPair<int, int>{0, 0};
    m_painter = new QPainter;
}

void GameGUI::paintEvent(QPaintEvent *event)
{

}

QPixmap GameGUI::resizeTexture(QPixmap texture, const QPair<int, int> sizeScene)
{
    if (texture.width() > sizeScene.first) {
        texture.scaled(sizeScene.first, texture.height());
    }

    if (texture.height() > sizeScene.second) {
        texture.scaled(texture.width(), sizeScene.second);
    }

    int countTexX = 0;

    for (int width = 0; width <= sizeScene.first; width += texture.width()) {
        countTexX ++;
    }

    int countTexY = 0;

    for (int height = 0; height <= sizeScene.second; height += texture.height()) {
        countTexY ++;
    }

    const int sumWidth = countTexX * texture.width();
    const int sumHeight = countTexY * texture.height();

    const int resizedWidth = static_cast<int>(texture.width() * sizeScene.first / sumWidth);
    const int resizedHeight = static_cast<int>(texture.height() * sizeScene.second / sumHeight);

    return texture.scaled(resizedWidth, resizedHeight);
}

void GameGUI::drawPlayer(const QPair<int, int> position, const QPair<int, int> size)
{
    m_painter->save();
    m_painter->drawPixmap(position.first, position.second, size.first, size.second, m_pmTexPlayer);
    m_painter->restore();
}

void GameGUI::drawScene(const QPair<int, int> position, const QPair<int, int> size)
{
    if (!m_isResized) {
        m_pmTexScene = resizeTexture(m_pmTexScene, size);
        m_isResized = true;
    }

    for (int posX = position.first; posX <= size.first; posX += m_pmTexScene.width()) {
        for (int posY = position.second; posY <= size.second; posY += m_pmTexScene.height()) {
            m_painter->save();
            m_painter->drawPixmap(posX, posY, m_pmTexScene.width(), m_pmTexScene.height(), m_pmTexScene);
            m_painter->restore();
        }
    }
}

void GameGUI::nextFrame(GameObjects gameObjects)
{
    m_gameObjects = gameObjects;
    update();

    m_painter->begin(this);
    m_painter->setRenderHint(QPainter::HighQualityAntialiasing);

    foreach (Scene scene, m_gameObjects.scene) {
//        qCInfo(m_lc) << QString("Emited scene's position: [%1:%2]").arg(QString::number(scene.x)).arg(QString::number(scene.y));
        drawScene(QPair<int, int>{scene.x, scene.y}, QPair<int, int>{scene.width, scene.height});
    }

    foreach (Player player, m_gameObjects.players) {
//        qCInfo(m_lc) << QString("Emited player's position: [%1:%2]").arg(QString::number(player.x)).arg(QString::number(player.y));
//        if ((abs(m_bufPosition.first - player.x) > 5) || (m_bufPosition.first == player.x)) {
//            qCInfo(m_lc) << QString("Freeze FPS[%1 -> %2] by X")
//                            .arg(QString::number(m_bufPosition.first))
//                            .arg(QString::number(player.x));
//        }

//        if ((abs(m_bufPosition.second - player.y) > 5) || (m_bufPosition.second == player.y)) {
//            qCInfo(m_lc) << QString("Freeze FPS[%1 -> %2] by Y")
//                            .arg(QString::number(m_bufPosition.second))
//                            .arg(QString::number(player.y));
//        }

        m_bufPosition = QPair<int, int>{player.x, player.y};
        drawPlayer(QPair<int, int>{player.x, player.y}, QPair<int, int>{player.width, player.height});
    }

    m_painter->end();
}

void GameGUI::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }

    onKeyboardEvent(event, true);
}

void GameGUI::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }

    onKeyboardEvent(event, false);
}

void GameGUI::mousePressEvent(QMouseEvent *)
{
    mouseClick(true);
}

void GameGUI::mouseReleaseEvent(QMouseEvent *)
{
    mouseClick(false);
}

void GameGUI::mouseMoveEvent(QMouseEvent *event)
{
    mouseMove(event->pos());
}

void GameGUI::onKeyboardEvent(QKeyEvent *event, bool isPressed)
{
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
