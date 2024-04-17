#include "system.h"


QVector<QString> GlobalStatus::allCardOwned = {
    "0001", "0001", "0001",
    "0002", "0002", "0002",
    "0003", "0004"
};
int GlobalStatus::playerMaxHp = 40;
int GlobalStatus::playerMaxEnergy = 3;


const int InGameSystem::_playerSlot = 0;

InGameSystem::InGameSystem()
    : _actionDisabled(0)
    , _playerEnergy(GlobalStatus::playerMaxEnergy)
    , _view(new gameboard())
    , _handCard({})
{
    // Init player
    auto player = new Player(InGameSystem::_playerSlot);
    _entities.push_back(player);
    connectSignalSlotForEntities(player);

    // Init enemies
    std::default_random_engine e;
    e.seed(std::time(nullptr));
    this->_enemyNum = e() % 3 + 1;

    for(int i = 0; i < this->_enemyNum; ++i)
    {
        _entities.push_back(new Enemy(i+1));
        connectSignalSlotForEntities(_entities[i+1]);
    }

    // Init cardStack
    auto list = GlobalStatus::allCardOwned;
    int len = list.size();
    for(int i = len-1; i >= 0; --i)
    {
        int ind = e() % (i+1);
        swap(list[ind], list[i]);
    }
    for(auto &i : this->_stack)
    {
        i = new CardStack();
    }
    this->_stack[DRAW]->push(list);
    for(auto &i : list)
    {
        emit addCardToStack(i);
    }

    // Init signals and slots
    QObject::connect(_view, SIGNAL(roundover()), this, SLOT(roundEnd()));

    // Init handCard
    for(int i = 0; i < 5; ++i)
    {
        const auto cardID = this->_stack[DRAW]->getPopOne();
        _handCard.push_back(cardID);
        emit addCardToHand(cardID);
    }
}

void InGameSystem::run()
{
    emit roundBegin(_playerSlot);
}

void InGameSystem::connectSignalSlotForEntities(Entity *entity)
{
    QObject::connect(entity, SIGNAL(requestHandleContext(Context*)), this, SLOT(handleContext(Context*)));
    QObject::connect(entity, SIGNAL(hpChanged(int,int)), _view, SLOT(updatehp(int,int)));
    QObject::connect(entity, SIGNAL(armorChanged(int,int)), _view, SLOT(updatearmor(int,int)));
    QObject::connect(entity, SIGNAL(buffChanged(int,bool,QString)), _view, SLOT(updatebuff(QString,int,bool)));
}

void InGameSystem::connectSignalSlotForView(gameboard *gameboard)
{
    QObject::connect(_view, SIGNAL(roundover()), this, SLOT(roundEnd()));
    QObject::connect(_view, SIGNAL(request_valid(QString,int*)), this, SLOT(handleCardValid(QString,int*)));
    QObject::connect(this, SIGNAL(updateEnergy(int)), _view, SLOT(updateenergy(int)));
    QObject::connect(this, SIGNAL(addCardToStack(QString)), _view->drawpile, SLOT(addcard(QString)));
    QObject::connect(this, SIGNAL(addCardToHand(QString)), _view->drawpile, SLOT(drawCard(QString)));
}

void InGameSystem::handleContext(Context *ctx)
{
    if(ctx->damageDone != 0)
    {
        ctx->from->attack(ctx, true);
    }
    if(ctx->armorGained != 0)
    {
        for(auto i : ctx->to)
        {
            i->gainArmor(ctx, true);
        }
    }
    if(ctx->buffGiven != "")
    {
        ctx->from->giveBuff(ctx, true);
    }
    if(ctx->actAltered != 0)
    {
        this->_actionDisabled ^= ctx->actAltered;
    }
    if(ctx->hpHealed != 0)
    {
        for(auto i : ctx->to)
        {
            i->heal(ctx);
        }
    }
}

void InGameSystem::playerUsingCard(int targetIndex, const QString &cardID)
{
    const auto info = CardSystem::getCardInfo(cardID);
    if(this->_playerEnergy < info.energy)
    {
        qFatal("In function %s: Unable to use card - cardID: %s - No energy", __FUNCTION__, info.id.toLatin1().data());
    }
    this->_playerEnergy -= info.energy;
    emit

    const auto actList = info.action;
    if(targetIndex >= _entities.size())
    {
        qFatal("In function %s: Unable to access to entity - index: %d", __FUNCTION__, targetIndex);
    }
    const auto target = _entities[targetIndex];
    if(target->isDead())
    {
        qFatal("In function %s: Unable to target a dead entity - index: %d", __FUNCTION__, targetIndex);
    }
    const auto actType = info.actType;
    if((actType & this->_actionDisabled) > 0)
    {
        qFatal("In function %s: Player using an invalid card - id: ", __FUNCTION__);
    }
    // process targetList
    const auto targetType = info.targetType;
    QVector<Entity*> targetList = {};
    switch (targetType) {
    case CardInfo::SELF:
        targetList.push_back(_entities[_playerSlot]);
        break;
    case CardInfo::ONE:
        targetList.push_back(_entities[targetIndex]);
        break;
    case CardInfo::ALL:
        for(int i = 1; i < _entities.size(); ++i)
        {
            targetList.push_back(_entities[i]);
        }
        break;
    case CardInfo::RANDOM:
        default_random_engine *e = new default_random_engine();
        targetList.push_back(_entities[(*e)() % (_entities.size() - 1) + 1]);
        delete e;
        break;
    }

    for(auto &i : actList)
    {
        auto ctx = i->getContext();
        ctx->from = this->_entities[_playerSlot];
        ctx->to = targetList;
        this->handleContext(ctx);
    }
    this->_stack[DROP]->push({cardID});
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

void InGameSystem::handleCardValid(QString cardID, int *valid)
{
    auto info = CardSystem::getCardInfo(cardID);
    int res = this->_actionDisabled & info.actType;
    *valid = !res;
}
