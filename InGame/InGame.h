#pragma once
#include "Basic.h"
#include "GameCard.h"
#include <qwidget.h>

class InGame : public QObject {
  Q_OBJECT
public:
  InGame(const QVector<Entity *> &enemy);
  void run();
  /*
  int enemyNumber() const;
  Entity* player() const;
  QVector<Entity*> enemies() const;
  */
public slots:
  // void handleCard(QString cardID, Entity* target);
private:
  Entity *_player;
  QVector<Entity *> _enemies;
  CardStack _draw, _drop;
};
/*
class InGameView : public QWidget
{
        Q_OBJECT
public:

private:
        QVector<CardView> cards;
};
*/
