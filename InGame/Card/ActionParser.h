#ifndef ACTIONPARSER_H
#define ACTIONPARSER_H

#include <QVector>
#include <QSet>
#include <QString>
#include <QDebug>
#include <functional>
#include "InGame/Context.h"

class BasicBuff;

class Action
{
public:
    using Act_t = int;
    enum Act : Act_t
    {
        ATTACK = (1 << 0),
        DEFEND = (1 << 1),
        BUFF = (1 << 2),
        NOT_FOUND = -1
    };
    static Act_t strToActType(const QString& str);
    virtual void act() = 0;
    Act_t getType() const;
    Context *getContex() const;
    Action(Act_t actType);
protected:
    Act_t _type;
    Context *_ctx;
private:
    static const QHash<QString, Act_t> _act;
};

class AttackAction : public Action
{
public:
    AttackAction(int damage);
    virtual void act() override;
};

class DefendAction : public Action
{
public:
    DefendAction(int armor);
    virtual void act() override;
};

class GiveBuffAction : public Action
{
public:
    GiveBuffAction(const QString &buffID);
    virtual void act() override;
};

class RemoveBuffAction : public Action
{
public:
    RemoveBuffAction(const QString &buffID);
    virtual void act() override;
};

class ActionParser
{
public:
    using res_t = QPair<bool, Action*>;
    using iter_t = QVector<QString>::iterator&;
    using end_t = const QVector<QString>::iterator;

    static QList<Action*> parse(const QString& str);

private:
    static const QHash<QString, std::function<res_t(iter_t, end_t)>> Functions;
    ActionParser() = delete;
};

#endif // ACTIONPARSER_H
