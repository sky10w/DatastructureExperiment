#include "Basic.h"

// Entity ---
Entity::Entity(bool isPlayer)
    : _isPlayer(isPlayer)
{}

void Entity::RoundBegin()
{
    for (auto eff : this->_buffList[BasicBuff::ON_ROUNDBEGIN])
	{
        eff->affect(nullptr);
	}
}
void Entity::RoundEnd()
{
    for (auto eff : this->_buffList[BasicBuff::ON_ROUNDEND])
	{
        eff->affect(nullptr);
    }
}

bool Entity::isPlayer() const
{
    return this->_isPlayer;
}

int Entity::getHp() const
{
    return this->_hp;
}

int Entity::getArmor() const
{
    return this->_armor;
}

void Entity::attack(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BasicBuff::ON_ATTACK])
        {
            item->affect(ctx);
        }
    }

    for(auto &tar: ctx->to)
    {
        tar->hurt(ctx, triggerBuff);
    }
}

void Entity::gainArmor(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BasicBuff::ON_GAINARMOR])
        {
            item->affect(ctx);
        }
    }

    this->_armor += ctx->armorGained;
}

void Entity::hurt(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BasicBuff::ON_HURT])
        {
            item->affect(ctx);
        }
    }

    int dealDamage = std::min(ctx->damageDone, this->_armor);
    ctx->damageDone -= dealDamage;
}

void Entity::giveBuff(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BasicBuff::ON_GIVEBUFF])
        {
            item->affect(ctx);
        }
    }
    for(auto &tar: ctx->to)
    {
        tar->getBuffed(ctx, triggerBuff);
    }
}

void Entity::getBuffed(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BasicBuff::ON_GETBUFFED])
        {
            item->affect(ctx);
        }
    }
    const auto buff = ctx->buffGiven;
    this->_buffList[buff->getType()].push_back(buff);
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

// BasicBuff ---
BasicBuff::BasicBuff(BuffType type)
    : _type(type)
{}

BasicBuff::BuffType BasicBuff::getType() const
{
    return this->_type;
}
// --- BasicEffect

// ModifyDamageBasicBuff ---
ModifyDamageBasicBuff::ModifyDamageBasicBuff(BuffType type)
    : BasicBuff(type)
{}
// --- ModifyDamageBasicEffect


// ModifyDamageByNumberBuff ---

void ModifyDamageByNumberBuff::affect(Context *ctx)
{
    ctx->damageDone += this->_incDamage;
}
// --- ModifyDamageByNumberBasicEffect



