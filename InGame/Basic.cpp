#include "Basic.h"

// Entity ---
const int Entity::_EventNumber = 5;

void Entity::Hurt(int damage, Context ctx) {
  ProcessState tempState;
  tempState.damage = damage;
  for (auto eff : this->_effectList[BasicEffect::ON_HURT]) {
    eff->affect(tempState);
  }
  // 进行实际的伤害计算
  if (this->_armor > tempState.damage) {
    this->_armor -= tempState.damage;
  } else {
    tempState.damage -= this->_armor;
    this->_armor = 0;
    this->_hp -= tempState.damage;
  }
}
void Entity::Attack(int damage, Context ctx) {
  ProcessState tempState;
  tempState.damage = damage;

  for (auto eff : this->_effectList[BasicEffect::ON_ATTACK]) {
    eff->affect(tempState);
  }
  for (auto tar : ctx.to) {
    tar->Hurt(tempState.damage, Context(tar, {this}));
  }
}
void Entity::RoundBegin() {
  ProcessState tempState;
  for (auto eff : this->_effectList[BasicEffect::ON_ROUNDBEGIN]) {
    eff->affect(tempState);
  }
}
void Entity::RoundEnd() {
  ProcessState tempState;
  for (auto eff : this->_effectList[BasicEffect::ON_ROUNDEND]) {
    eff->affect(tempState);
  }
}
// --- Entity

// Enemy ---
void Enemy::RoundMid() {}
// --- Enemy

// Player ---
void Player::RoundBegin() {
  /// TODO Write something...
  this->Entity::RoundBegin();
}

void Player::RoundMid() {
  /// TODO Write something...
}
// --- Player

ProcessState::ProcessState() : damage(0), armor(0), hp(0) {}

// BasicEffect ---
BasicEffect::BasicEffect(Context ctx, EffectType type)
    : _ctx(ctx), _type(type) {}
// --- BasicEffect

// DoDamageBasicEffect ---
DoDamageBasicEffect::DoDamageBasicEffect(Context ctx, EffectType type,
                                         int damage)
    : BasicEffect(ctx, type), _damage(damage) {}
void DoDamageBasicEffect::affect(ProcessState &state) {
  _ctx.from->Attack(state.damage, _ctx);
}
// --- DoDamageBasicEffect

// ModifyDamageBasicEffect ---
ModifyDamageBasicEffect::ModifyDamageBasicEffect(Context ctx, EffectType type)
    : BasicEffect(ctx, type) {}
// --- ModifyDamageBasicEffect

// ModifyDamageByNumberBasicEffect ---
ModifyDamageByNumberBasicEffect::ModifyDamageByNumberBasicEffect(
    Context ctx, EffectType type, int reducedDamage)
    : ModifyDamageBasicEffect(ctx, type), _incDamage(reducedDamage) {}
void ModifyDamageByNumberBasicEffect::affect(ProcessState &state) {
  state.damage += this->_incDamage;
}
// --- ModifyDamageByNumberBasicEffect

// Context ---
Context::Context(Entity *const from, QVector<Entity *> const to)
    : from(from), to(to) {}
// --- Context

// EffectDistributor ---
void EffectDistributor::distributeBasicEffect(BasicEffect *eff) {
  ProcessState tempState;
  switch (eff->_type) {
  case BasicEffect::IMMEDIATE:
    // Execute immediately
    eff->affect(tempState);
    break;

  default:
    // Insert into _onAttack
    auto &ctx = eff->_ctx;
    for (auto &tar : ctx.to) {
      tar->_effectList[eff->_type].push_back(eff);
    }
    break;
  }
}
// --- EffectDistributor
