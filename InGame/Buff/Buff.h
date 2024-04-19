#ifndef BUFFH_H
#define BUFFH_H

#include <QHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

#include "InGame/globalOptions.h"
#include "InGame/Context.h"
#include "InGame/Card/ActionParser.h"

class BasicBuff;

/// BuffInfo(id, className, buffName, description, type, iconPath, (BasicBuff*)buff)

struct BuffInfo
{
    enum BuffType : int
    {
        ON_ROUNDBEGIN = 0,
        ON_ROUNDEND,
        ON_GIVEBUFF,
        ON_GETBUFFED,
        ON_ATTACK,
        ON_GAINARMOR,
        ON_HURT,

        NOT_FOUND
    };
    QString id;
    QString className;
    QString buffName;
    QString description;
    BuffType type;
    QString iconPath;
    BasicBuff* buff;
};

class BasicBuff
{
public:
    BasicBuff(BuffInfo::BuffType type);
    virtual bool isValid() const;
    virtual BuffInfo::BuffType getType() const;
    virtual QString getID() const;

    virtual BasicBuff* getCopy() = 0;
    virtual void affect(Context *ctx) = 0;
    virtual void degrade() = 0;

protected:
    QString _id;
    BuffInfo::BuffType _type;
    int _degree;
private:
    virtual void setID(QString id);
    friend class BuffSystem;
};



class ModifyDamageBasicBuff : public BasicBuff
{
public:
    ModifyDamageBasicBuff(BuffInfo::BuffType type);
    virtual BasicBuff* getCopy() override = 0;
    virtual void affect(Context *ctx) override = 0;
    virtual void degrade() override = 0;
};

class ModifyDamageByNumberBuff : public ModifyDamageBasicBuff
{
public:
    ModifyDamageByNumberBuff(BuffInfo::BuffType type, int incDamage);
    virtual BasicBuff *getCopy() override;
    virtual void affect(Context *ctx) override;
    virtual void degrade() override;

protected:
    int _incDamage;
};

class ModifyDamageByPercentBuff : public ModifyDamageBasicBuff
{
public:
    ModifyDamageByPercentBuff(BuffInfo::BuffType type, int percent, int degradeLevel);
    virtual BasicBuff* getCopy() override;
    virtual void affect(Context *ctx) override;
    virtual void degrade() override;

protected:
    int _percent;
};

class ReadyToAttackBuff : public BasicBuff
{
public:
    ReadyToAttackBuff(BuffInfo::BuffType type, int degradeLevel);
    virtual BasicBuff* getCopy()override;
    virtual void affect(Context* ctx) override;
    virtual void degrade() override;
};

class HealBuff : public BasicBuff
{
public:
    HealBuff(BuffInfo::BuffType type, int healAmount);
    virtual BasicBuff *getCopy() override;
    virtual void affect(Context* ctx) override;
    virtual void degrade() override;
protected:
    int _healAmount;
};


#endif // BUFFH_H
