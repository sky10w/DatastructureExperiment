#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include "GameCard.h"
#include "Basic.h"
#include "Buff.h"

class System : public QObject
{
    Q_OBJECT
public:
    System();
    void run();

signals:
    void sendEntityList(QVector<Entity*> list);
    void nextRoundHint(QHash<Entity*, EntityAct::Act_t> act);
    void entityAct(EntityAct::Act_t act);

    void roundBegin(const Entity* entity);
    void hpChanged(int oldHp, int newHp);
    void armorChanged(int oldArmor, int newArmor);
    void handCardChanged(QVector<QString> oldCards, QVector<QString> newCards);
public slots:
    virtual void playerUsingCard(const QString& cardID);
    virtual void roundEnd();

signals:
    void sendHandCard(const QVector<QString>& card);
    void sendCardStackInfo(QVector<QString> list);
public slots:
    virtual void needCardStack(bool isDrawStack);
    virtual void needHandCard();

private:
    Entity* _player;
    QVector<Entity*> _enemies;
    Entity* _cur;
    CardStack* _stack[2];
    QVector<QString> _handCard;
};


#endif // SYSTEM_H
