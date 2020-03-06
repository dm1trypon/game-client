#ifndef GAMEGUI_H
#define GAMEGUI_H

#include "consts.h"

#include <QOpenGLWidget>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QLoggingCategory>
#include <QKeyEvent>
#include <QTimer>
#include <QPixmap>

using namespace GameContent;
using namespace GameData;

class GameGUI : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GameGUI(QWidget *parent = nullptr);

public slots:
    void nextFrame(GameObjects gameObjects);

signals:
    void mouseClick(bool);
    void mouseMove(QPoint);

    void keyboard(const QString &, bool);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_isResized = false;

    QPainter *m_painter = nullptr;

    QPixmap m_pmTexScene;
    QPixmap m_pmTexPlayer;

    GameObjects m_gameObjects;
    QLoggingCategory m_lc;

    QPair<int, int> m_bufPosition;

    void onKeyboardEvent(QKeyEvent *event, bool isPressed);
    void onMouseEvent(bool isPressed);
    void drawScene(const QPair<int, int> position, const QPair<int, int> size);
    void drawPlayer(const QPair<int, int> position, const QPair<int, int> size);

    QPixmap resizeTexture(QPixmap texture, const QPair<int, int> sizeScene);
};

#endif // GAMEGUI_H
