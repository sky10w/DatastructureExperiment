#ifndef BUFFH_H
#define BUFFH_H

#include <QHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include "InGame/globalOptions.h"
#include "InGame/Context.h"

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

public:
    BasicBuff(BuffInfo::BuffType type);
    virtual void affect(Context *ctx) = 0;
    virtual void degrade() = 0;
    virtual BuffInfo::BuffType getType() const;

protected:
    BuffInfo::BuffType _type;
};



class ModifyDamageBasicBuff : public BasicBuff
{
public:
    ModifyDamageBasicBuff(BuffInfo::BuffType type);
    virtual void affect(Context *ctx) override = 0;
    virtual void degrade() override = 0;
};

class ModifyDamageByNumberBuff : public ModifyDamageBasicBuff
{
public:
    ModifyDamageByNumberBuff(BuffInfo::BuffType type, int incDamage);
    virtual void affect(Context *ctx) override;
    virtual void degrade() override;

protected:
    int _incDamage;
};


#endif // BUFFH_H
