#pragma once
#include <QWidget>
#include <QVector>
#include <algorithm>
#include <random>

#include "Context.h"
#include "Card/CardSystem.h"
#include "Buff/BuffSystem.h"

#define MyDebug qDebug() << "[In function" << (__FUNCTION__ + QString("]:")).toLatin1().data()

struct GlobalRandomEngine
{
    static std::default_random_engine getRandom;
    static void init();
};

struct Context;
// using Context = QMap<QString, QVariant>;
class BasicBuff;
class Entity;
class BuffDistributor;


class Entity : public QObject
{
	Q_OBJECT
public:
    explicit Entity(bool isPlayer, int index, int hp);

    virtual bool isPlayer() const;
    virtual int getMaxHp() const;
    virtual int getHp() const;
    virtual int getArmor() const;
    virtual bool isDead() const;

    // Actions

    virtual void roundBegin();
    virtual void roundEnd();

    virtual void heal(Context* ctx);
    virtual void removeBuff(Context* ctx);
    virtual void buffRemoved(Context* ctx);

    virtual void attack(Context* ctx, bool triggerBuff);
    virtual void gainArmor(Context* ctx, bool triggerBuff);
    virtual void hurt(Context* ctx, bool triggerBuff);
    virtual void giveBuff(Context* ctx, bool triggerBuff);
    virtual void getBuffed(Context* ctx, bool triggerBuff);

signals:
    void requestHandleContext(Context* ctx); // to system
    void hpChanged(int id, int delta); // to view
    void armorChanged(int id, int delta); // to view
    void buffChanged(QString buffI, int strength, int id); // to view

protected:
    int _maxHp;
	int _hp;
	int _armor;
    bool _isPlayer;
    int _id;

    std::list<BasicBuff*> _buffList;

    // std::list<BasicBuff*> _buffList[7];
    void handleBuffList(Context* ctx, BuffInfo::BuffType type);
};

class Enemy : public Entity
{
    Q_OBJECT
public:
    Enemy(int index, int hp);
    virtual void setNextAction(QPair<Action::Act_t, Context*> _next) = 0;
    virtual QPair<Action::Act_t, Context*> getNextAction() = 0;
};

class NormalEnemy : public Entity
{
	Q_OBJECT
public:
    NormalEnemy(int index, int hp);
    virtual void setNextAction(QPair<Action::Act_t, Context*> _next) final;
    virtual QPair<Action::Act_t, Context*> getNextAction() final;

protected:
    QPair<Action::Act_t, Context*> _nextAction;
};

class Boss : public NormalEnemy
{
    Q_OBJECT
public:
    Boss(int index, int hp);
};

class Player : public Entity
{
	Q_OBJECT
public:
    Player(int index, int hp);
};


