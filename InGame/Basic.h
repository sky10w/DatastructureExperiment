#pragma once
#include <qwidget.h>
#include <qvector.h>

#include "GameCard.h"

struct Context;
// using Context = QMap<QString, QVariant>;
class BasicEffect;
class Entity;
class EffectDistributor;

/// Context(from, to(vector), damageDone, armorGained, effectGiven)
struct Context
{
    Entity* from;
    QVector<Entity*> to;
    int damageDone;
    int armorGained;
    BasicEffect* effectGiven;
};

// class Action : public QObject
// {
//     Q_OBJECT
// public:
//     explicit Action(Entity* owner);
//     virtual void act(Context* ctx, bool triggerEffect) = 0;

// protected:
//     Entity* _owner;
//     std::list<BasicEffect*> _effectList;
// };

// class Hurt : public Action
// {
//     Q_OBJECT
// public:
//     virtual void act(Context* ctx, bool triggerEffect) override;
// };

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
    virtual void attack(Context* ctx, bool triggerEffect);
    virtual void gainArmor(Context* ctx, bool triggerEffect);
    virtual void hurt(Context* ctx, bool triggerEffect);

signals:
    void roundBegin(const Entity* entity);
    void usingEffect();
    void hpChanged(int oldHp, int newHp);
    void armorChanged(int oldArmor, int newArmor);
    void sendBuffInfo();
    void sendCardStackInfo(QVector<CardInfo*> list);
    void sendCardInfo();
    // void sendEntityList(QVector<Entity*> list) function is done by higher system

public slots:
    virtual void usingCard(const QString& CardID);
    virtual void roundEnd();
    // virtual void checkCardStack() function is done by higher system
    virtual void needBuffInfo();
    // virtual void needCardInfo() function is implemented by Player class

protected:
	int _hp;
	int _armor;
    bool _isPlayer;

	static const int _EventNumber;
    std::list<BasicEffect*> _effectList[6];
    // Action* _hurt;
    // Action* _attack;

	friend class EffectDistributor;
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




class BasicEffect : public QObject
{
	Q_OBJECT
public:
	enum EffectType : int
	{
		ON_ROUNDBEGIN = 0,
		ON_ROUNDEND,
		ON_ATTACK,
        ON_GAINARMOR,
		ON_HURT,
		IMMEDIATE
	};
public:
    BasicEffect(EffectType type);
    virtual void affect(Context *ctx) = 0;
protected:
	EffectType _type;
	friend class EffectDistributor;
};

class DoDamageBasicEffect : public BasicEffect
{
	Q_OBJECT
public:
    DoDamageBasicEffect(EffectType type, int damage);
    virtual void affect(Context *ctx) override;
protected:
	int _damage;
};

class ModifyDamageBasicEffect : public BasicEffect
{
	Q_OBJECT
public:
    ModifyDamageBasicEffect(EffectType type);
    virtual void affect(Context *ctx) override = 0;
protected:
	int* _damagePtr;
};

class ModifyDamageByNumberBasicEffect : public ModifyDamageBasicEffect
{
	Q_OBJECT
public:
	ModifyDamageByNumberBasicEffect(Context ctx, EffectType type, int reducedDamage);
    virtual void affect(Context *ctx) override;
protected:
    int _incDamage;
};

class BuffInfo
{
public:
    QString id;
    QString name;
    QString description;
    QString path;
};

class EffectDistributor : public QObject
{
	Q_OBJECT
public:
    virtual void distributeBasicEffect(BasicEffect* eff, Context* ctx);
};


