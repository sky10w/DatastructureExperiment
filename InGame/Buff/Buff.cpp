#include "Buff.h"

BasicBuff::BasicBuff(BuffInfo::BuffType type)
    : _type(type)
    , _degree(1)
{}

bool BasicBuff::isValid() const
{
    return this->_degree <= 0;
}

BuffInfo::BuffType BasicBuff::getType() const
{
    return this->_type;
}

QString BasicBuff::getID() const
{
    return this->_id;
}

void BasicBuff::setID(QString id)
{
    this->_id = id;
}


ModifyDamageBasicBuff::ModifyDamageBasicBuff(BuffInfo::BuffType type)
    : BasicBuff(type)
{}


ModifyDamageByNumberBuff::ModifyDamageByNumberBuff(BuffInfo::BuffType type, int incDamage)
    : ModifyDamageBasicBuff(type)
    , _incDamage(incDamage)
{}

void ModifyDamageByNumberBuff::affect(Context *ctx)
{
    ctx->damageDone += this->_incDamage;
}

void ModifyDamageByNumberBuff::degrade()
{
    // Do nothing here
}

ModifyDamageByPercentBuff::ModifyDamageByPercentBuff(BuffInfo::BuffType type, int percent)
    : ModifyDamageBasicBuff(type)
    , _percent(percent)
{
    this->_degree = 2;
}

void ModifyDamageByPercentBuff::affect(Context *ctx)
{
    ctx->damageDone = (double)ctx->damageDone * (1.0 + (double)this->_percent * 0.01);
}

void ModifyDamageByPercentBuff::degrade()
{
    this->_degree--;
}

ReadyToAttackBuff::ReadyToAttackBuff(BuffInfo::BuffType type, int degradeLevel)
    : BasicBuff(type)
{
    this->_degree = degradeLevel;
}

void ReadyToAttackBuff::affect(Context *ctx)
{
    degrade();
    if(this->_degree == 0)
    {
        ctx->buffGiven = "0004";
        // emit degreeToZero();
    }
}

void ReadyToAttackBuff::degrade()
{
    this->_degree--;
}
