#ifndef CORE_H
#define CORE_H

#include "consts.h"
#include "sceneobjects.h"

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>

using namespace GameData;

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    void onNextMessage(const QJsonObject gameDataObj);

private slots:
    void onNextFrame();

signals:
    void nextFrame(GameObjects);

private:
    SceneObjects m_sceneObjects;

    bool m_isSkip = false;

    QLoggingCategory m_lc;
    QTimer *m_tmrNextFrame = nullptr;

    template<typename TypeObject>
    QList<TypeObject> makeObjectsList(const QJsonArray goArr, const QString &goName, TypeObject);
};

#endif // CORE_H
