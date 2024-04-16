#include "system.h"


QVector<QString> GlobalStatus::allCardOwned = {
    "0001", "0001", "0001",
    "0002", "0002", "0002",
    "0003", "0004"
};
int GlobalStatus::playerHp = 40;
int GlobalStatus::playerEnergy = 3;


const int InGameSystem::_playerSlot = 0;

InGameSystem::InGameSystem()
    // Init actionDisabled, handCard
    : _actionDisabled(0)
    , _handCard({})
{
    // Init entities, enemyNum, curEntity, stack
    auto player = new Player(InGameSystem::_playerSlot);
    _entities.push_back(player);

    std::default_random_engine e;
    e.seed(std::time(nullptr));
    this->_enemyNum = e() % 3 + 1;

    for(int i = 0; i < this->_enemyNum; ++i)
    {
        _entities.push_back(new Enemy(i+1));
    }

    auto list = GlobalStatus::allCardOwned;
    int len = list.size();
    for(int i = len-1; i >= 0; --i)
    {
        int ind = e() % (i+1);
        swap(list[ind], list[i]);
    }

    this->_stack[DRAW]->push(list);
}

void InGameSystem::run()
{
    auto oldHandCard = _handCard;
    _handCard = this->_stack[DRAW]->getPopN(5);
    emit handCardChanged(oldHandCard, _handCard);
    emit roundBegin(_playerSlot);
}

void InGameSystem::connectSignalSLot(Entity *entity)
{
    QObject::connect(entity, SIGNAL(actionChanged(Action::Act_t,bool)), this, SLOT(updateAction(Action::Act_t,bool)));
    QObject::connect(entity, SIGNAL(hpChanged(int,int)), /**/, SLOT(updatehp(int,int)));
    QObject::connect(entity, SIGNAL(armorChanged(int,int)), /*this*/, SLOT(updatearmor(int,int)));
    QObject::connect(entity, SIGNAL(buffChanged(int,bool,QString)), /*this*/, SLOT(updatebuff(int,bool,QString)));
}

void InGameSystem::updateAction(Action::Act_t type, bool isRestrict)
{
    if(isRestrict)
        this->_actionDisabled |= type;
    else
        this->_actionDisabled &= ~type;
}

void InGameSystem::playerUsingCard(const QString &cardID, int targetIndex)
{
    const auto res = CardSystem::getCardInfo(cardID);
    const auto actList = res.action;
    if(targetIndex > _entities.size())
    {
        qFatal("In function \"%s\": Unable to access to entities[%d]", __FUNCTION__, targetIndex);
    }
    const auto target = _entities[targetIndex];
    if(target->isDead())
    {
        qFatal("In function \"%s\": Unable to target a dead entity - index: ", __FUNCTION__, targetIndex);
    }
    for(auto &i : actList)
    {
        auto ctx = i->getContext();
    }
}

// Round end for player's round
// Need to execute the following enemies' action
void InGameSystem::roundEnd()
{
    for(int i = 1; i <= _enemyNum; ++i)
    {
        _curEntity = i;
        _entities[i]->roundBegin();

        emit roundBegin(i);

        _entities[i]->roundEnd();

        /// TODO
        QThread::msleep(2000);
    }
    _curEntity = 0;
    emit roundBegin(0);
}

void InGameSystem::needCardValid(const QString &id)
{
    bool valid = (CardSystem::getCardInfo(id).actType & (~this->_actionDisabled)) > 0;
    emit sendCardValid(id, valid);
}

void InGameSystem::needCardStack(bool isDrawStack)
{
    const auto allcards = _stack[isDrawStack]->getAll();
    emit sendCardStackInfo(allcards);
}

void InGameSystem::needHandCard()
{
    emit sendHandCard(this->_handCard);
}
