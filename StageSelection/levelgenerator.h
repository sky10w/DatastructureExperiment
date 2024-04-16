#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <QObject>
#include <QtWidgets>
#include <algorithm>
#include <ctime>
#include <random>
#include <QPoint>
#include <QDebug>

struct lnode{
    QPoint position;
    int leveltype;
    bool currentlevel;
};


class LevelGenerator : public QObject
{
    Q_OBJECT
public:

    explicit LevelGenerator(QObject *parent = nullptr);

signals:
    void levelGenerated(QList<lnode> nodes, QList<QPair<int, int>> edges,int branch);

public slots:
    void generateLevel();
};

#endif // LEVELGENERATOR_H
