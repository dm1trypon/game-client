#ifndef GAMEGUI_H
#define GAMEGUI_H

#include <QGLWidget>

class GameGUI : public QGLWidget
{
public:
    explicit GameGUI(QWidget *parent = nullptr);

protected:
      void initializeGL();
      void resizeGL(int w, int h);
      void paintGL();
};

#endif // GAMEGUI_H
