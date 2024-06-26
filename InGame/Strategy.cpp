#include "Strategy.h"

Action::Act_t NormalStrategy::generateAction(Context *ctx)
{
    const int res = GlobalRandomEngine::getRandom() % 2;
    if (res == 1) {
        ctx->damageDone = 6;
        return Action::ATTACK;
    }
    ctx->to = {ctx->from};
    ctx->armorGained = 3;
    return Action::DEFEND;
}

Action::Act_t BossStrategy::generateAction(Context *ctx)
{
    const int remainHp = ctx->from->getHp();
    const int maxHp = ctx->from->getMaxHp();
    if (remainHp > maxHp / 2) {
        ctx->damageDone = 7;
        return Action::ATTACK;
    }
    int res = GlobalRandomEngine::getRandom() % 2;
    if (res == 1) {
        ctx->to = {ctx->from};
        ctx->buffGiven = "0006";
        return Action::BUFF;
    }
    ctx->damageDone = 12;
    return Action::ATTACK;
}
