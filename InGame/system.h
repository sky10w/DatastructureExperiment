#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QThread>
#include "Card/CardSystem.h"
#include "Buff/BuffSystem.h"
#include "Basic.h"
#include "gameboard/gameboard.h"
#include <random>

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
    void connectSignalSlotForEntities(Entity* entity);

private:
    static const int _playerSlot;

public slots:

signals:
    void nextRoundHint(QHash<Entity*, Action::Act_t> act);
    void entityAct(Action::Act_t act);

    void roundBegin(int id);
    void hpChanged(int id, int delta);
    void armorChanged(int id, int delta);
    void handCardChanged(QVector<QString> oldCards, QVector<QString> newCards);
public slots:
    virtual void handleContext(Context* ctx); // from Entity
    virtual void playerUsingCard(int targetIndex, const QString& cardID);
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
    gameboard* _view;
    QVector<QString> _handCard;
};


#endif // SYSTEM_H
