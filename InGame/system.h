#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QThread>
#include "Card/CardSystem.h"
#include "Buff/BuffSystem.h"
#include "Basic.h"
#include <random>

class View;

class GlobalStatus
{
public:
    static QVector<QString> allCardOwned;
    static int playerHp;
    static int playerEnergy;
};


class InGameSystem : public QObject
{
    Q_OBJECT
public:
    enum cardstack_t : bool {
        DROP = false,
        DRAW = true
    };
    InGameSystem();
    void run();
    void connectSignalSLot(Entity* entity);

private:
    static const int _playerSlot;

public slots:
    virtual void updateAction(Action::Act_t type, bool isRestrict);

signals:
    void nextRoundHint(QHash<Entity*, Action::Act_t> act);
    void entityAct(Action::Act_t act);

    void roundBegin(int id);
    void hpChanged(int id, int delta);
    void armorChanged(int id, int delta);
    void handCardChanged(QVector<QString> oldCards, QVector<QString> newCards);
public slots:
    virtual void playerUsingCard(const QString& cardID, int targetIndex);
    /// Round end for player's round
    virtual void roundEnd();

signals:
    void sendCardValid(QString cardID, bool isValid);
    void sendEntityList(QVector<Entity *> list);
    void sendHandCard(const QVector<QString> &card);
    void sendCardStackInfo(QVector<QString> list);
public slots:
    virtual void needCardValid(const QString& id);
    virtual void needCardStack(bool isDrawStack);
    virtual void needHandCard();

private:
    int _actionDisabled;
    QVector<Entity*> _entities;
    int _enemyNum;
    int _curEntity;
    CardStack* _stack[2];
    View* _view;
    QVector<QString> _handCard;
};


#endif // SYSTEM_H
