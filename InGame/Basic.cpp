#include "Basic.h"

// Entity ---
Entity::Entity(bool isPlayer, int index)
    : _isPlayer(isPlayer)
    , _id(index)
{}

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

bool Entity::isDead() const
{
    return this->_hp <= 0;
}

void Entity::roundBegin()
{
    Context* ctx = new Context{this, {this}};
    for (auto eff : this->_buffList[BuffInfo::ON_ROUNDBEGIN])
	{
        eff->affect(ctx);
	}
    delete ctx;
}

void Entity::roundEnd()
{
    Context* ctx = new Context{this, {this}};
    for (auto eff : this->_buffList[BuffInfo::ON_ROUNDEND])
	{
        eff->affect(ctx);
    }
    delete ctx;
}

void Entity::heal(Context *ctx)
{
    this->_hp += ctx->hpHealed;
}

void Entity::removeBuff(Context *ctx)
{
    for(auto &tar : ctx->to)
    {
        tar->buffRemoved(ctx);
    }
}

void Entity::buffRemoved(Context *ctx)
{
    auto type = BuffSystem::getBuffInfo(ctx->buffGiven).type;
    auto &tarList = this->_buffList[type];
    // find the corresponding buff
    const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffGiven);
    for(auto i = tarList.begin(); i != tarList.end();)
    {
        auto cur = *i;
        if(cur->getID() == ctx->buffGiven)
        {
            i = tarList.erase(i);
            continue;
        }
        ++i;
    }
    emit buffChanged(0, ctx->buffGiven);
}

void Entity::restrictAction(Context *ctx)
{
    emit actionChanged(ctx->actAltered, true);
}

void Entity::unrestrictAction(Context *ctx)
{
    emit actionChanged(ctx->actAltered, false);
}

void Entity::attack(Context *ctx, bool triggerBuff)
{
    if(triggerBuff)
    {
        for(auto &item :this->_buffList[BuffInfo::ON_ATTACK])
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
        for(auto &item :this->_buffList[BuffInfo::ON_GAINARMOR])
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
        for(auto &item :this->_buffList[BuffInfo::ON_HURT])
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
        for(auto &item :this->_buffList[BuffInfo::ON_GIVEBUFF])
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
        for(auto &item :this->_buffList[BuffInfo::ON_GETBUFFED])
        {
            item->affect(ctx);
        }
    }
    const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffGiven);
    auto buff = BuffParser::parse(buffInfo.className);
    /// TODO
    this->_buffList[buff->getType()].push_back(buff);
}
// --- Entity

// Enemy ---
Enemy::Enemy(int index)
    : Entity(false, index)
{}

// --- Enemy


// Player ---

Player::Player(int index)
    : Entity(true, index)
{}

// --- Player


