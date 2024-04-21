#include "Basic.h"

std::default_random_engine GlobalRandomEngine::getRandom;

// Entity ---
Entity::Entity(bool isPlayer, int index, int hp)
    : _maxHp(hp)
    , _hp(hp)
    , _armor(0)
    , _isPlayer(isPlayer)
    , _id(index)
{
}

bool Entity::isPlayer() const
{
    return this->_isPlayer;
}

int Entity::getMaxHp() const
{
    return this->_maxHp;
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
    Context *ctx = new Context{};
    ctx->from = this;
    ctx->to = {this};
    this->handleBuffList(ctx, BuffInfo::ON_ROUNDBEGIN);
    emit requestHandleContext(ctx);
    delete ctx;
}

void Entity::roundEnd() {
    Context *ctx = new Context{};
    ctx->from = this;
    ctx->to = {this};
    this->handleBuffList(ctx, BuffInfo::ON_ROUNDEND);
    emit requestHandleContext(ctx);
    delete ctx;
}

void Entity::heal(Context *ctx) {
  this->_hp += ctx->hpHealed;
  emit hpChanged(this->_id, ctx->hpHealed);
}

void Entity::removeBuff(Context *ctx)
{
    for (auto &tar : ctx->to) {
        tar->buffRemoved(ctx);
    }
}

void Entity::buffRemoved(Context *ctx)
{
    // auto type = BuffSystem::getBuffInfo(ctx->buffGiven).type;
    auto &tarList = this->_buffList;
    // find the corresponding buff
    const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffRemoved);
    for (auto i = tarList.begin(); i != tarList.end();) {
        auto cur = *i;
        if (cur->getID() == ctx->buffRemoved) {
            i = tarList.erase(i);
            emit buffChanged(ctx->buffRemoved, -99999, this->_id);
            break;
        }
        ++i;
    }
}

void Entity::attack(Context *ctx, bool triggerBuff) {
    qDebug() << this->_id << "Attack - damage" << ctx->damageDone;
    if (triggerBuff) {
        this->handleBuffList(ctx, BuffInfo::ON_ATTACK);
    }

    for (auto &tar : ctx->to) {
        tar->hurt(ctx, triggerBuff);
    }
}

void Entity::gainArmor(Context *ctx, bool triggerBuff) {
    MyDebug << this->_id << "Defend - defend" << ctx->armorGained;
    if (triggerBuff) {
        this->handleBuffList(ctx, BuffInfo::ON_GAINARMOR);
    }

    this->_armor += ctx->armorGained;
    emit this->armorChanged(this->_id, ctx->armorGained);
}

void Entity::hurt(Context *ctx, bool triggerBuff) {
    if (triggerBuff) {
        this->handleBuffList(ctx, BuffInfo::ON_HURT);
    }

    int dealToArmorDamage = std::min(ctx->damageDone, this->_armor);
    this->_armor -= dealToArmorDamage;
    ctx->damageDone -= dealToArmorDamage;
    qDebug() << this->_id << "Hurt on armor! Damage:" << dealToArmorDamage;
    emit this->armorChanged(this->_id, -dealToArmorDamage);

    if(ctx->damageDone <= 0) return;
    qDebug() << this->_id << "Hurt! Damage:" << ctx->damageDone;
    this->_hp -= ctx->damageDone;
    emit this->hpChanged(this->_id, -ctx->damageDone);
}

void Entity::giveBuff(Context *ctx, bool triggerBuff) {
    if (triggerBuff) {
        this->handleBuffList(ctx, BuffInfo::ON_GIVEBUFF);
    }
    for (auto &tar : ctx->to) {
        qDebug() << "Entity giving Buff - id:" << ctx->buffGiven;
        tar->getBuffed(ctx, triggerBuff);
    }
}


void Entity::getBuffed(Context *ctx, bool triggerBuff)
{
    if (triggerBuff) {
        this->handleBuffList(ctx, BuffInfo::ON_GETBUFFED);
    }
    const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffGiven);
    auto buffCopy = buffInfo.buff->getCopy();
    qDebug() << "Entity got Buff - id:" << ctx->buffGiven << "- type:" << buffCopy->getType();
    // this->_buffList[buffInfo.type].push_back(buffCopy);
    this->_buffList.push_back(buffCopy);
    emit buffChanged(ctx->buffGiven, buffCopy->isValid(), this->_id);
}

void Entity::handleBuffList(Context *ctx, BuffInfo::BuffType type)
{
    auto &tarBuffList = this->_buffList;
    for (auto eff = tarBuffList.begin(); eff != tarBuffList.end();)
    {
        qDebug() << (*eff)->getID() << "affecting";
        (*eff)->affect(ctx, type);
        if(!(*eff)->isValid())
        {
            qDebug() << (*eff)->getID() << "degrade and has been removed";
            emit this->buffChanged((*eff)->getID(), -99999, this->_id);
            eff = tarBuffList.erase(eff);
        } else {
            ++eff;
        }
    }
}

Player::Player(int index, int hp)
    : Entity(true, index, hp)
{}

NormalEnemy::NormalEnemy(int index, int hp)
    : Entity(false, index, hp)
{}

QPair<Action::Act_t, Context *> NormalEnemy::getNextAction()
{
    return this->_nextAction;
}

void NormalEnemy::setNextAction(QPair<Action::Act_t, Context *> _next)
{
    this->_nextAction = _next;
}

Boss::Boss(int index, int hp)
    : NormalEnemy(index, hp)
{}

void GlobalRandomEngine::init()
{
    GlobalRandomEngine::getRandom.seed(std::time(NULL));
}
