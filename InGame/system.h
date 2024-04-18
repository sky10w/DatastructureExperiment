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
    static int playerMaxHp;
    static int playerMaxEnergy;
    static int playerMaxHandCard;
};


class InGameSystem : public QWidget
{
    Q_OBJECT
public:
    enum cardstack_t : int { DROP = 0, DRAW = 1 };
    InGameSystem(QWidget* parent = nullptr);
    void initSystem(bool isBoss);
    void run();
    void connectSignalSlotForEntities(Entity* entity);
    void connectSignalSlotForView();
    void shuffle();
    bool drawCard();
    void gameend(bool isWin);
    int checkGameover();

private:
    static const int _playerSlot;

signals:
    void nextRoundHint(QHash<Entity*, Action::Act_t> act);
    void entityAct(Action::Act_t act);

    void sendShuffle();
    void setEnergy(int energy);
    void updateEnergy(int delta);
    void roundBegin();
    void hpChanged(int id, int delta);
    void armorChanged(int id, int delta);
    void gameover(bool isWin);
    void changeScene(int scene);
public slots:
    virtual void handleContext(Context* ctx); // from Entity
    virtual void playerUsingCard(int cardIndex, int targetIndex);
    /// Round end for player's round
    virtual void roundEnd();

signals:
    void addCardToStack(QString cardID);
    void addCardToHand(QString cardID);

    // void sendEntityList(QVector<Entity *> list);
    // void sendHandCard(const QVector<QString> &card);
    // void sendCardStackInfo(QVector<QString> list);
public slots:
    // virtual void needCardValid(const QString& id);
    // virtual void needCardStack(bool isDrawStack);
    // virtual void needHandCard();
    virtual void handleCardValid(QString cardID, int* valid);

private:
    int _actionDisabled;
    QVector<Entity*> _entities;
    int _enemyNum;
    int _curEntity;
    int _playerEnergy;
    bool _inited;
    int _gameover;
    CardStack* _stack[2];
    QGraphicsView* _gView;
    QGraphicsScene* _scene;
    gameboard* _view;
    QVector<QString> _handCard;
};


#endif // SYSTEM_H
