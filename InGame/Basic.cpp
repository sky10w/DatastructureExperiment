#include "Basic.h"

// Entity ---
Entity::Entity(bool isPlayer, int index) : _isPlayer(isPlayer), _id(index) {}

bool Entity::isPlayer() const { return this->_isPlayer; }

int Entity::getHp() const { return this->_hp; }

int Entity::getArmor() const { return this->_armor; }

bool Entity::isDead() const { return this->_hp <= 0; }

void Entity::roundBegin() {
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

void Entity::removeBuff(Context *ctx) {
  for (auto &tar : ctx->to) {
    tar->buffRemoved(ctx);
  }
}

void Entity::buffRemoved(Context *ctx) {
  auto type = BuffSystem::getBuffInfo(ctx->buffGiven).type;
  auto &tarList = this->_buffList[type];
  // find the corresponding buff
  const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffGiven);
  for (auto i = tarList.begin(); i != tarList.end();) {
    auto cur = *i;
    if (cur->getID() == ctx->buffGiven) {
      i = tarList.erase(i);
      continue;
    }
    ++i;
  }
  /// TODO
  emit buffChanged(this->_id, -99999, ctx->buffGiven);
}

void Entity::attack(Context *ctx, bool triggerBuff) {
  if (triggerBuff) {
    this->handleBuffList(ctx, BuffInfo::ON_ATTACK);
  }

  for (auto &tar : ctx->to) {
    tar->hurt(ctx, triggerBuff);
  }
}

void Entity::gainArmor(Context *ctx, bool triggerBuff) {
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

  int dealDamage = std::min(ctx->damageDone, this->_armor);
  ctx->damageDone -= dealDamage;
  emit this->hpChanged(this->_id, -dealDamage);
}

void Entity::giveBuff(Context *ctx, bool triggerBuff) {
  if (triggerBuff) {
    this->handleBuffList(ctx, BuffInfo::ON_GIVEBUFF);
  }
  for (auto &tar : ctx->to) {
    tar->getBuffed(ctx, triggerBuff);
  }
}

void Entity::getBuffed(Context *ctx, bool triggerBuff) {
  if (triggerBuff) {
    this->handleBuffList(ctx, BuffInfo::ON_GETBUFFED);
  }
  const auto buffInfo = BuffSystem::getBuffInfo(ctx->buffGiven);
  auto buff = BuffParser::parse(buffInfo.className);
  /// TODO
  this->_buffList[buff->getType()].push_back(buff);
}

void Entity::handleBuffList(Context *ctx, BuffInfo::BuffType type) {
  auto &tarBuffList = this->_buffList[type];
  for (auto eff = tarBuffList.begin(); eff != tarBuffList.end();) {
    (*eff)->affect(ctx);
    (*eff)->degrade();
    if (!(*eff)->isValid())
      eff = tarBuffList.erase(eff);
  }
}
Player::Player(int index) : Entity(true, index) {}

Enemy::Enemy(int index) : Entity(false, index) {}
