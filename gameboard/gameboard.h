#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "InGame/Card/CardSystem.h"
#include "InGame/Card/GameCard.h"
#include "ingame/Buff/BuffSystem.h"
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
#include <QTimer>
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
  Buff(QString id, int strength);
  QString iconpath;
  QString description;
  QString uuid;
  int strength; // buff strength
};

class BuffView : public QObject //状态视图
{
  Q_OBJECT
public:
  QGraphicsView buffview;
  map<QString, Buff *> Buffs;
  map<QString, BuffIcon *> bufficon;
  QGraphicsScene buffscene;
  int MAXBUFFSUM = 10;
  BuffView();
  //~BuffView();

  void init(int posx, int posy);
  void print();
  void update_buff(QString uuid, int delta);
  void updateview();
signals:

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
  void initasplayer(int id);
  void initasenemy(int id);
  void updatehpview();
  void updatearmorview();
  void update_buff(QString id, int delta);

  void update_HP(int delta);
  void update_armor(int delta);
  void update_action(int actionid);
  // QRectF boundingRect() const override;
};

class CardView : public QObject,
                 public QGraphicsPixmapItem //卡牌视图
{
  Q_OBJECT
public:
  // bool valid;
  bool inhands;
  int curposx, curposy;
  int cardtype;
  QGraphicsLineItem arrow;
  QGraphicsTextItem description;
  QString uuid;
  CardInfo info;
  void init();

signals:
  void playcard(int id); //这是前端内部的

public slots:
  // void get_valid(bool isvalid); // valid

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
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

  void updatecard();         //更新手牌排布//这是前端内部的
  void consumecard(int id);  //对id打出卡牌//这是前端内部的
  void carddraw(CardView *); //抽卡//这是前端内部的
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

  void update();
signals:
  void shuffle();
  void send_card_to_hands(CardView *card);
public slots:
  void drawcard(QString uuid); //抽到一张uuid的牌
  void addcard(QString uuid);
};
class gameboard : public MyOpenGLWidget {
  Q_OBJECT
public:
  explicit gameboard(QWidget *parent = nullptr);
  QGraphicsScene scene;
  QGraphicsView view;
  QPushButton DiscardPileButton;
  QPushButton DrawPileButton;
  QPushButton SettingsButton;
  QPushButton EnergyButton;
  QPushButton EndButton;
  QVector<QPushButton *> buttons;
  EntityView *Player;
  map<int /*id*/, EntityView *> Enemy;
  // EntityView Enemy2;
  HandsView *myhands;
  DrawPileView *drawpile;
  DiscardPileView *discardpile;
  bool playerround;
  int energy;
  void DiscardPileButton_clicked();
  void DrawPileButton_clicked();
  void SettingsButton_clicked();
  void EnergyButton_clicked();
  void EndButton_clicke();
  void updateenergyview();

  void setanimation(QString path, int time, int posx, int posy);
signals:
  void roundover();
  void request_valid(QString uuid, int *valid); //请求valid值
public slots:
  void roundbegin();
  void discardcard(CardView *card); //这是前端内部的
  void shuffle();                   //洗牌
  void updateenergy(int delta);     //能量+=delta
  void setenergy(int x);            //设定能量为x
  void updatebuff(QString buffid, int strength,
                  int id); //对name施加一个强度为strength的uuidbuff
  void updatehp(int id, int delta);    // name的hp+=delta
  void updatearmor(int id, int delta); // name的armor+=delta
  void initenemy(int id, QString name, int HP_MAX);  //初始化一个敌人
  void initplayer(int id, int HP_MAX); //初始化一个玩家

  // int get
};
#endif // GAMEBOARD_H
