#ifndef GAMEGUI_H
#define GAMEGUI_H

#include "consts.h"

#include <QOpenGLWidget>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QLoggingCategory>
#include <QKeyEvent>

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
    void keyboard(const QString &, bool);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    GameObjects m_gameObjects;
    QLoggingCategory m_lc;

    void onKeyboardEvent(QKeyEvent *event, bool isPressed);
    void paint(QPainter *painter, QPaintEvent *event, QPair<int, int> position, QPair<int, int> size, const QString &texPath);
};

#endif // GAMEGUI_H
