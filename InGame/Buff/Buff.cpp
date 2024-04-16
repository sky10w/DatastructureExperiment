#include "Buff.h"

BasicBuff::BasicBuff(BuffInfo::BuffType type)
    : _type(type)
{}

BuffInfo::BuffType BasicBuff::getType() const
{
    return this->_type;
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
