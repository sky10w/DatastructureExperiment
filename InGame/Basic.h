#pragma once
#include <QWidget>
#include <QVector>
#include <algorithm>

#include "Context.h"
#include "Card/CardSystem.h"
#include "Buff/BuffSystem.h"

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
    void buffChanged(int id, int strength, QString buffID); // to view

protected:
	int _hp;
	int _armor;
    bool _isPlayer;
    int _id;

    std::list<BasicBuff*> _buffList[7];
    void handleBuffList(Context* ctx, BuffInfo::BuffType type);
};

class Enemy : public Entity
{
	Q_OBJECT
public:
    Enemy(int index, int hp);

};

class Player : public Entity
{
	Q_OBJECT
public:
    Player(int index, int hp);
};


