#pragma once
#include <qwidget.h>
#include <qvector.h>

struct Context;
class BasicEffect;
class Entity;
class EffectDistributor;

class Entity : public QObject
{
	Q_OBJECT
public:
	virtual void Hurt(int damage, Context ctx);
	virtual void Attack(int damage, Context ctx);
	virtual void RoundBegin();
	virtual void RoundMid() = 0;
	virtual void RoundEnd();
protected:
	int _hp;
	int _armor;

	static const int _EventNumber;
	std::list<BasicEffect*> _effectList[5];

	friend class EffectDistributor;
};

class Enemy : public Entity
{
	Q_OBJECT
public:
	virtual void RoundMid() override;
};

class Player : public Entity
{
	Q_OBJECT
public:
	virtual void RoundBegin() override;
	virtual void RoundMid() override;
};


struct Context
{
	Context(Entity* const from, QVector<Entity*> const to);
	Context(const Context& r) = default;
	Entity* from;
	QVector<Entity*> to;
};

struct ProcessState
{
	int damage;
	int armor;
	int hp;
	ProcessState();
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
		ON_HURT,
		IMMEDIATE
	};
public:
	BasicEffect(Context ctx, EffectType type);
	virtual void affect(ProcessState& state) = 0;
protected:
	Context _ctx;
	EffectType _type;
	friend class EffectDistributor;
};

class DoDamageBasicEffect : public BasicEffect
{
	Q_OBJECT
public:
	DoDamageBasicEffect(Context ctx, EffectType type, int damage);
	virtual void affect(ProcessState& state) override;
protected:
	int _damage;
};

class ModifyDamageBasicEffect : public BasicEffect
{
	Q_OBJECT
public:
	ModifyDamageBasicEffect(Context ctx, EffectType type);
	virtual void affect(ProcessState& state) override = 0;
protected:
	int* _damagePtr;
};

class ModifyDamageByNumberBasicEffect : public ModifyDamageBasicEffect
{
	Q_OBJECT
public:
	ModifyDamageByNumberBasicEffect(Context ctx, EffectType type, int reducedDamage);
	virtual void affect(ProcessState& state) override;
protected:
	int _incDamage;
};

class EffectDistributor : public QObject
{
	Q_OBJECT
public:
	virtual void distributeBasicEffect(BasicEffect* eff);
};


