#pragma once
#include <QWidget>
#include <QVector>

#include "GameCard.h"

struct Context;
// using Context = QMap<QString, QVariant>;
class BasicBuff;
class Entity;
class BuffDistributor;

namespace EntityAct
{
    using Act_t = int;
    enum Act : int
    {
        ATTACK = (1 << 0),
        DEFEND = (1 << 1),
        BUFF = (1 << 2),
    };
}

/// Context(from, to(vector), damageDone, armorGained, effectGiven)
struct Context
{
    Entity* from;
    QVector<Entity*> to;
    int damageDone;
    int armorGained;
    BasicBuff* buffGiven;
};

class Entity : public QObject
{
	Q_OBJECT
public:
    explicit Entity(bool isPlayer);
	virtual void RoundBegin();
	virtual void RoundMid() = 0;
	virtual void RoundEnd();

    virtual bool isPlayer() const;
    virtual int getHp() const;
    virtual int getArmor() const;

    // Actions

    virtual void attack(Context* ctx, bool triggerBuff);
    virtual void gainArmor(Context* ctx, bool triggerBuff);
    virtual void hurt(Context* ctx, bool triggerBuff);
    virtual void giveBuff(Context* ctx, bool triggerBuff);
    virtual void getBuffed(Context* ctx, bool triggerBuff);

protected:
	int _hp;
	int _armor;
    bool _isPlayer;
    QString _id;

    std::list<BasicBuff*> _buffList[7];

    friend class BuffDistributor;
};

class Enemy : public Entity
{
	Q_OBJECT
public:
    Enemy();
	virtual void RoundMid() override;
};

class Player : public Entity
{
	Q_OBJECT
public:
    Player();
	virtual void RoundBegin() override;
	virtual void RoundMid() override;
private:
    QList<QString> _cards;
};



class BasicBuff : public QObject
{
	Q_OBJECT
public:
    enum BuffType : int
	{
		ON_ROUNDBEGIN = 0,
		ON_ROUNDEND,
        ON_GIVEBUFF,
        ON_GETBUFFED,
		ON_ATTACK,
        ON_GAINARMOR,
        ON_HURT
	};
public:
    BasicBuff(BuffType type);
    virtual void affect(Context *ctx) = 0;
    virtual BuffType getType() const;
protected:
    BuffType _type;
};

class ModifyDamageBasicBuff : public BasicBuff
{
	Q_OBJECT
public:
    ModifyDamageBasicBuff(BuffType type);
    virtual void affect(Context *ctx) override = 0;
};

class ModifyDamageByNumberBuff : public ModifyDamageBasicBuff
{
	Q_OBJECT
public:
    ModifyDamageByNumberBuff(BuffType type, int incDamage);
    virtual void affect(Context *ctx) override;
protected:
    int _incDamage;
};



