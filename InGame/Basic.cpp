#include "Basic.h"

// Entity ---
const int Entity::_EventNumber = 5;

// void Entity::Hurt(int damage, Context ctx)
// {
// 	ProcessState tempState;
// 	tempState.damage = damage;
// 	for (auto eff : this->_effectList[BasicEffect::ON_HURT])
// 	{
// 		eff->affect(tempState);
// 	}
// 	// ����ʵ�ʵ��˺�����
// 	if (this->_armor > tempState.damage)
// 	{
// 		this->_armor -= tempState.damage;
// 	}
// 	else
// 	{
// 		tempState.damage -= this->_armor;
// 		this->_armor = 0;
// 		this->_hp -= tempState.damage;
// 	}
// }
// void Entity::Attack(int damage, Context ctx)
// {
// 	ProcessState tempState;
// 	tempState.damage = damage;

// 	for (auto eff : this->_effectList[BasicEffect::ON_ATTACK])
// 	{
// 		eff->affect(tempState);
// 	}
// 	for (auto tar : ctx.to)
// 	{
// 		tar->Hurt(tempState.damage, Context(tar, { this }));
// 	}
// }
Entity::Entity(bool isPlayer)
    : _isPlayer(isPlayer)
{}

void Entity::RoundBegin()
{
	for (auto eff : this->_effectList[BasicEffect::ON_ROUNDBEGIN])
	{
        eff->affect(nullptr);
	}
}
void Entity::RoundEnd()
{
	for (auto eff : this->_effectList[BasicEffect::ON_ROUNDEND])
	{
        eff->affect(nullptr);
    }
}

void Entity::attack(Context *ctx, bool triggerEffect)
{
    if(triggerEffect)
    {
        for(auto &item :this->_effectList[BasicEffect::ON_ATTACK])
        {
            item->affect(ctx);
        }
    }

    for(auto &tar: ctx->to)
    {
        tar->hurt(ctx, triggerEffect);
    }
}

void Entity::gainArmor(Context *ctx, bool triggerEffect)
{
    if(triggerEffect)
    {
        for(auto &item :this->_effectList[BasicEffect::ON_GAINARMOR])
        {
            item->affect(ctx);
        }
    }

    this->_armor += ctx->armorGained;
}

void Entity::hurt(Context *ctx, bool triggerEffect)
{
    if(triggerEffect)
    {
        for(auto &item :this->_effectList[BasicEffect::ON_HURT])
        {
            item->affect(ctx);
        }
    }

    int dealDamage = std::min(ctx->damageDone, this->_armor);
    ctx->damageDone -= dealDamage;
}
// --- Entity

// Enemy ---
Enemy::Enemy()
    : Entity(false)
{}

void Enemy::RoundMid()
{

}
// --- Enemy


// Player ---

Player::Player()
    : Entity(true)
{}

void Player::RoundBegin()
{
	/// TODO Write something...
	this->Entity::RoundBegin();
}

void Player::RoundMid()
{
	/// TODO Write something...
}
// --- Player

// BasicEffect ---
BasicEffect::BasicEffect(EffectType type)
    : _type(type)
{}
// --- BasicEffect


// DoDamageBasicEffect ---
DoDamageBasicEffect::DoDamageBasicEffect(EffectType type, int damage)
    : BasicEffect(type)
	, _damage(damage)
{}
void DoDamageBasicEffect::affect(Context *ctx)
{
    ctx->from->attack(ctx, true);
}
// --- DoDamageBasicEffect


// ModifyDamageBasicEffect ---
ModifyDamageBasicEffect::ModifyDamageBasicEffect(EffectType type)
    : BasicEffect(type)
{}
// --- ModifyDamageBasicEffect


// ModifyDamageByNumberBasicEffect ---
ModifyDamageByNumberBasicEffect::ModifyDamageByNumberBasicEffect(Context ctx, EffectType type, int reducedDamage)
    : ModifyDamageBasicEffect(type)
	, _incDamage(reducedDamage)
{}
void ModifyDamageByNumberBasicEffect::affect(Context *ctx)
{
    ctx->damageDone += this->_incDamage;
}
// --- ModifyDamageByNumberBasicEffect


// EffectDistributor ---
void EffectDistributor::distributeBasicEffect(BasicEffect* eff, Context* ctx)
{
	switch (eff->_type)
	{
	case BasicEffect::IMMEDIATE:
		// Execute immediately
        eff->affect(ctx);
		break;

	default:
        // Distribute to corresponding effect list
        for (auto& tar : ctx->to)
        {
			tar->_effectList[eff->_type].push_back(eff);
		}
		break;
	}
}
// --- EffectDistributor


/// Deprecated
// // Action ---
// Action::Action(Entity *owner)
//     : _owner(owner)
//     , _effectList()
// {}
// // --- Action

// void Hurt::act(Context *ctx, bool triggerEffect)
// {
//     int initialHp = this->_owner->getHp();
//     this->_owner->setHp(initialHp - ctx->damageDone);
// }
