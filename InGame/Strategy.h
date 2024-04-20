#ifndef STRATEGY_H
#define STRATEGY_H

#include "Basic.h"

class Strategy
{
public:
    virtual Action::Act_t generateAction(Context *ctx) = 0;
};

class NormalStrategy : public Strategy
{
public:
    virtual Action::Act_t generateAction(Context *ctx) override;
};

class BossStrategy : public Strategy
{
public:
    virtual Action::Act_t generateAction(Context *ctx) override;
};

#endif // STRATEGY_H
