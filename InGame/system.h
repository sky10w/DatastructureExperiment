#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include "Card/CardSystem.h"
#include "Buff/BuffSystem.h"
#include "Basic.h"
#include <random>

class System : public QObject
{
    Q_OBJECT
public:
    System();
    void run();

signals:
    void nextRoundHint(QHash<Entity*, Action::Act_t> act);
    void entityAct(Action::Act_t act);

    void roundBegin(int id);
    void hpChanged(int id, int delta);
    void armorChanged(int id, int delta);
    void handCardChanged(QVector<QString> oldCards, QVector<QString> newCards);
    void updateHandCardValid(QVector<bool> valid);
public slots:
    virtual void playerUsingCard(const QString& cardID);
    virtual void roundEnd();

signals:
    void sendEntityList(QVector<Entity*> list);
    void sendHandCard(const QVector<QString>& card);
    void sendCardStackInfo(QVector<QString> list);
public slots:
    virtual void needCardStack(bool isDrawStack);
    virtual void needHandCard();

private:
    QVector<Entity*> _entities;
    int _enemyNum;
    Entity* _cur;
    CardStack* _stack[2];
    QVector<QString> _handCard;
};


#endif // SYSTEM_H
