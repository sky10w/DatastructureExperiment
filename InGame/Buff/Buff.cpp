#include "Buff.h"

BasicBuff::BasicBuff(BuffInfo::BuffType type)
    : _type(type)
    , _degree(1)
{}

bool BasicBuff::isValid() const
{
    return this->_degree > 0;
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

BasicBuff *ModifyDamageByNumberBuff::getCopy()
{
    return new ModifyDamageByNumberBuff(*this);
}

void ModifyDamageByNumberBuff::affect(Context *ctx, BuffInfo::BuffType situation)
{
    if (this->_type != situation) {
        return;
    }
    ctx->damageDone += this->_incDamage;
}

void ModifyDamageByNumberBuff::degrade()
{
    // Do nothing here
}

ModifyDamageByPercentBuff::ModifyDamageByPercentBuff(BuffInfo::BuffType type,
                                                     int percent,
                                                     int degradeLevel)
    : ModifyDamageBasicBuff(type)
    , _percent(percent)
{
    this->_degree = degradeLevel;
}

BasicBuff *ModifyDamageByPercentBuff::getCopy()
{
    return new ModifyDamageByPercentBuff(*this);
}

void ModifyDamageByPercentBuff::affect(Context *ctx, BuffInfo::BuffType situation)
{
    if (situation == BuffInfo::ON_ROUNDEND)
        degrade();
    if (this->_type != situation) {
        return;
    }
    const int temp = ctx->damageDone;
    ctx->damageDone = (double)ctx->damageDone * (1.0 + (double)this->_percent * 0.01);
    qDebug() << "ModifyDamageByPercent - initial damage:" << temp << "- after damage:" << ctx->damageDone;
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

BasicBuff *ReadyToAttackBuff::getCopy()
{
    return new ReadyToAttackBuff(*this);
}

void ReadyToAttackBuff::affect(Context *ctx, BuffInfo::BuffType situation)
{
    qDebug() << "ReadyToAttackBuff affecting";
    if (this->_type != situation) {
        return;
    }
    this->degrade();
    if (!this->isValid()) {
        ctx->buffGiven = "+0004";
    }
}

void ReadyToAttackBuff::degrade()
{
    this->_degree--;
}

HealBuff::HealBuff(BuffInfo::BuffType type, int healAmount)
    : BasicBuff(type)
    , _healAmount(healAmount)
{}

BasicBuff *HealBuff::getCopy()
{
    return new HealBuff(*this);
}

void HealBuff::affect(Context *ctx, BuffInfo::BuffType situation)
{
    ctx->hpHealed = this->_healAmount;
}

void HealBuff::degrade()
{
    // Do Nothing
}
