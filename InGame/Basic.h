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
    explicit Entity(bool isPlayer);

    virtual bool isPlayer() const;
    virtual int getHp() const;
    virtual int getArmor() const;

    // Actions

    virtual void roundBegin();
    virtual void roundEnd();
    virtual void heal(Context* ctx);
    virtual void removeBuff(Context* ctx);

    virtual void attack(Context* ctx, bool triggerBuff);
    virtual void gainArmor(Context* ctx, bool triggerBuff);
    virtual void hurt(Context* ctx, bool triggerBuff);
    virtual void giveBuff(Context* ctx, bool triggerBuff);
    virtual void getBuffed(Context* ctx, bool triggerBuff);

signals:
    void hpChanged(int delta);
    void armorChanged(int delta);
    void buffChanged(bool dir, QString buffID);

protected:
	int _hp;
	int _armor;
    bool _isPlayer;
    QString _id;

    std::list<BasicBuff*> _buffList[7];
};

class Enemy : public Entity
{
	Q_OBJECT
public:
    Enemy();
};

class Player : public Entity
{
	Q_OBJECT
public:
    Player();
private:
    QList<QString> _cards;
};





