#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "InGame/GameCard.h"
#include "myopenglwidget.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QList>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QPushButton>
#include <QQueue>
#include <QScrollBar>
#include <QSet>
#include <QSharedPointer>
#include <QString>
#include <QTextDocument>
#include <QToolTip>
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <iostream>
#include <map>
#include <set>
using namespace std;

class HandsView;
class CardView;
class BuffIcon : public QObject, public QGraphicsPixmapItem {
  Q_OBJECT
public:
  QString description;
  QGraphicsSimpleTextItem strength;

protected:
};

class Buff {
public:
  Buff(int uuid, int strength);
  int uuid;
  int strength; // buff strength
};

class BuffView : public QObject //状态视图
{
  Q_OBJECT
public:
  QGraphicsView buffview;
  map<int, Buff *> Buffs;
  map<int, BuffIcon *> bufficon;
  QGraphicsScene buffscene;
  int MAXBUFFSUM;
  BuffView();
  //~BuffView();

  void init(int posx, int posy);
  void print();
  void updatebuff(int id, int delta);
  void updateset(set<pair<int, int>> x);
  void updateview();
signals:
  void getBuff();

  // void hoverEnterEvent(QGraphicsSceneEvent *event);
  // void hoverLeaveEvent(QGraphicsSceneEvent *event);
};

class EntityView : public QObject,
                   public QGraphicsPixmapItem //实体视图
{
  Q_OBJECT
public:
  int id;
  int HP;
  int HP_MAX;
  int posx, posy;
  int armor;
  int width = 260;
  int height = 260;
  QString name;
  QWidget *parent;
  BuffView mybuff;
  QString type; // enemy or player
  QGraphicsLineItem hpbar;
  QGraphicsSimpleTextItem hpnumber;
  QGraphicsPixmapItem armoricon;
  QGraphicsSimpleTextItem armornumber;
  QGraphicsPixmapItem action; // only for enemy
  void init(int hp);
  void initasplayer();
  void initasenemy();
  void updatehpview();
  void updatearmorview();
  void updatebuff(int id, int delta);
  void update_HP(int delta);
  void update_armor(int delta);
  void update_action(int id);
  // QRectF boundingRect() const override;
};
class MyGraphicsLineItem : public QGraphicsLineItem {
  // Q_OBJECT
  // protected:
  QRectF boundingRect() const override;
};

class CardView : public QObject,
                 public QGraphicsPixmapItem //卡牌视图
{
  Q_OBJECT
public:
  bool valid;
  bool inhands;
  int curposx, curposy;
  int cardtype;
  QGraphicsLineItem arrow;
  QGraphicsTextItem description;
  QString uuid;
  CardInfo info;
  void init();

signals:
  void playcard(int id);
  void request_valid(int cardtype);
public slots:
  void get_valid(bool isvalid);

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

class HandsView : public QObject //手牌场景
{
  Q_OBJECT
public:
  QGraphicsView *handsview;
  QGraphicsScene *handsscene;
  QVector<CardView *> hands;
  void init();

public slots:
  void updatecard();         //更新手牌排布
  void consumecard(int id);  //对id打出卡牌
  void carddraw(CardView *); //抽卡
signals:
  void discardcard(CardView *card);
  void playcard(int index, int id); //对id打出第index张牌
};

class DiscardPileView : public QGraphicsScene {
  Q_OBJECT
public:
  QGraphicsPixmapItem description;
  QGraphicsPixmapItem background;
  multimap<QString, CardView *> st;
  void addcard(CardView *card);
  void init();
  void clean();
  void update();
};
class DrawPileView : public QGraphicsScene {
  Q_OBJECT
public:
  QGraphicsPixmapItem description;
  QGraphicsPixmapItem background;
  multimap<QString, CardView *> st;
  void init();
  void removecard(QString uuid);
  void addcard(QString uuid);
  void update();
signals:
  void shuffle();
  void send_card_to_hands(CardView *card);
public slots:
  void drawcard(QString uuid);
};
class gameboard : public MyOpenGLWidget {
  Q_OBJECT
public:
  explicit gameboard(MyOpenGLWidget *parent = nullptr);
  QGraphicsScene scene;
  QGraphicsView view;
  QPushButton DiscardPileButton;
  QPushButton DrawPileButton;
  QPushButton SettingsButton;
  QPushButton EnergyButton;
  QPushButton EndButton;
  QVector<QPushButton *> buttons;
  EntityView *Player;
  vector<EntityView *> Enemy;
  // EntityView Enemy2;
  HandsView *myhands;
  DrawPileView *drawpile;
  DiscardPileView *discardpile;
  int energy;
  void DiscardPileButton_clicked();
  void DrawPileButton_clicked();
  void SettingsButton_clicked();
  void EnergyButton_clicked();
  void EndButton_clicke();
  void updateenergyview();

signals:
  void roundover();
public slots:
  void discardcard(CardView *card);
  void shuffle();
  void updateenergy(int x);
  void updatebuff(int buffid, int strength, int objectid = 0);
  void updatehp(int id, int delta);
  void updatearmor(int id, int delta);
  // int get
};
#endif // GAMEBOARD_H
