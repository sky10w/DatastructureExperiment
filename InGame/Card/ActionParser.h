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
    using ActMap_t = std::map<int, Action::Act_t>;
    enum _Act : Act_t {
        ATTACK = (1 << 0),
        DEFEND = (1 << 1),
        BUFF = (1 << 2),
        ACTION = (1 << 3),
        NOT_FOUND = -1
    };
    static Act_t strToActType(const QString& str);

    virtual void debug() = 0;
    virtual void act(Context* ctx) = 0;

    Act_t getType() const;
    Action(Act_t actType);
protected:
    Act_t _type;
private:
    static const QHash<QString, Act_t> _act;
};

class AttackAction : public Action
{
public:
    AttackAction(int damage);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    int _damage;
};

class DefendAction : public Action
{
public:
    DefendAction(int armor);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    int _armor;
};

class GiveBuffAction : public Action
{
public:
    GiveBuffAction(const QString &buffID);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    QString _buffID;
};

class RemoveBuffAction : public Action
{
public:
    RemoveBuffAction(const QString &buffID);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    QString _buffID;
};

class RestrictAction : public Action
{
public:
    RestrictAction(Action::Act_t type);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    Action::Act_t _type;
};

class UnrestrictAction : public Action
{
public:
    UnrestrictAction(Action::Act_t type);
    virtual void debug() override;
    virtual void act(Context* ctx) override;
protected:
    Action::Act_t _type;
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
