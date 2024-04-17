#include "system.h"

QVector<QString> GlobalStatus::allCardOwned = {
    "0001", "0001", "0001", "0002", "0003", "0003", "0003", "0004", "0005"
};
int GlobalStatus::playerMaxHp = 40;
int GlobalStatus::playerMaxEnergy = 10;

const int InGameSystem::_playerSlot = 0;

InGameSystem::InGameSystem()
    : _actionDisabled(0), _playerEnergy(GlobalStatus::playerMaxEnergy),
      _view(new gameboard()), _handCard({}) {
  // Init signals and slots
  connectSignalSlotForView();

  // Init player
  auto player = new Player(InGameSystem::_playerSlot, 40);
  _entities.push_back(player);
  connectSignalSlotForEntities(player);
  this->_view->initplayer(InGameSystem::_playerSlot, GlobalStatus::playerMaxHp);

  // Init enemies
  std::default_random_engine e;
  e.seed(std::time(nullptr));
  this->_enemyNum = e() % 3 + 1;

  for (int i = 0; i < this->_enemyNum; ++i) {
    _entities.push_back(new Enemy(i + 1, 20));
    connectSignalSlotForEntities(_entities[i + 1]);
    this->_view->initenemy(i + 1, 20);
  }
}

void InGameSystem::run() {
  _view->resize(1280, 720);
  _view->show();

  // Init cardStack
  auto list = GlobalStatus::allCardOwned;
  int len = list.size();
  std::default_random_engine e;
  for (int i = len - 1; i >= 0; --i) {
    int ind = e() % (i + 1);
    swap(list[ind], list[i]);
  }
  for (auto &i : this->_stack) {
    i = new CardStack();
  }
  this->_stack[DRAW]->push(list);
  for (auto &i : list) {
    emit addCardToStack(i);
  }

  // Init handCard
    for (int i = 0; i < 5; ++i) {
        const auto cardID = this->_stack[DRAW]->getPopOne();
        _handCard.push_front(cardID);
        emit addCardToHand(cardID);
    }

    this->_actionDisabled = 0;
    emit setEnergy(GlobalStatus::playerMaxEnergy);
    emit roundBegin();
}

// Round end for player's round
// Need to execute the following enemies' action
void InGameSystem::roundEnd() {
    for (int i = 1; i <= _enemyNum; ++i) {
        _curEntity = i;
        _entities[i]->roundBegin();

        /// Test
        auto ctx = new Context{};
        ctx->from = _entities[i];
        ctx->to = {_entities[0]};
        ctx->damageDone = 5;
        this->handleContext(ctx);

        _entities[i]->roundEnd();

        /// TODO
        QThread::msleep(1000);
    }
    _curEntity = 0;

    if (this->_stack[DRAW]->empty()) {
        shuffle();
    }
    for(int i = 0; i < 2; ++i)
    {
        const auto cardID = this->_stack[DRAW]->getPopOne();
        if(cardID == "-1") break;
        _handCard.push_front(cardID);
        emit addCardToHand(cardID);
    }
    emit setEnergy(GlobalStatus::playerMaxEnergy);
    this->_actionDisabled = 0;
    emit roundBegin();

}

void InGameSystem::connectSignalSlotForEntities(Entity *entity) {
  QObject::connect(entity, SIGNAL(requestHandleContext(Context*)), this, SLOT(handleContext(Context*)));
  QObject::connect(entity, SIGNAL(hpChanged(int,int)), _view, SLOT(updatehp(int,int)));
  QObject::connect(entity, SIGNAL(armorChanged(int,int)), _view, SLOT(updatearmor(int,int)));
  QObject::connect(entity, SIGNAL(buffChanged(QString,int,int)), _view, SLOT(updatebuff(QString,int,int)));
}

void InGameSystem::connectSignalSlotForView() {
  QObject::connect(_view, SIGNAL(roundover()), this, SLOT(roundEnd()));
  QObject::connect(_view, SIGNAL(request_valid(QString,int*)), this, SLOT(handleCardValid(QString,int*)));
  QObject::connect(this, SIGNAL(updateEnergy(int)), _view,SLOT(updateenergy(int)));
  QObject::connect(this, SIGNAL(addCardToStack(QString)), _view->drawpile, SLOT(addcard(QString)));
  QObject::connect(this, SIGNAL(addCardToHand(QString)), _view->drawpile, SLOT(drawcard(QString)));
  QObject::connect(this, SIGNAL(roundBegin()), _view, SLOT(roundbegin()));
  QObject::connect(this, SIGNAL(sendShuffle()), _view, SLOT(shuffle()));
  QObject::connect(this, SIGNAL(setEnergy(int)), _view, SLOT(setenergy(int)));
  QObject::connect(_view->myhands, SIGNAL(playcard(int,int)), this, SLOT(playerUsingCard(int,int)));
}

void InGameSystem::shuffle() {
  emit this->sendShuffle();
  const auto list = this->_stack[DROP]->getPopAll();
  this->_stack[DRAW]->push(list);
}

void InGameSystem::handleContext(Context *ctx) {
    if (ctx->damageDone != 0) {
        ctx->from->attack(ctx, true);
    }
    if (ctx->armorGained != 0) {
        for (auto i : ctx->to) {
            i->gainArmor(ctx, true);
        }
    }
    if (ctx->buffGiven != "") {
        if(ctx->buffGiven[0] == '+')
        {
            auto &str = ctx->buffGiven;
            const auto iter = next(str.begin());
            str.erase(str.begin(), iter);
            ctx->from->giveBuff(ctx, true);
        }
        else if(ctx->buffGiven[0] == '-')
        {
            auto &str = ctx->buffGiven;
            const auto iter = next(str.begin());
            str.erase(str.begin(), iter);
            ctx->from->removeBuff(ctx);
        }
    }
    if (ctx->actAltered != 0) {
        this->_actionDisabled ^= ctx->actAltered;
    }
    if (ctx->hpHealed != 0) {
        for (auto i : ctx->to) {
            i->heal(ctx);
        }
    }
}

void InGameSystem::playerUsingCard(int cardIndex,int targetIndex) {
    const auto cardID = this->_handCard[cardIndex];
    qDebug() << "Player using card - id:" << cardID << "index:" << cardIndex;
    const auto info = CardSystem::getCardInfo(cardID);
    if (this->_playerEnergy < info.energy)
    {
        qFatal("In function %s: Unable to use card - cardID: %s - No energy", __FUNCTION__, info.id.toLatin1().data());
    }
    this->_playerEnergy -= info.energy;
    emit this->updateEnergy(-info.energy);


    const auto actList = info.action;
    if (targetIndex >= _entities.size())
    {
        qFatal("In function %s: Unable to access to entity - index: %d", __FUNCTION__, targetIndex);
    }
    const auto target = _entities[targetIndex];
    if (target->isDead())
    {
        qFatal("In function %s: Unable to target a dead entity - index: %d", __FUNCTION__, targetIndex);
    }
    const auto actType = info.actType;
    if ((actType & this->_actionDisabled) > 0)
    {
        qFatal("In function %s: Player using an invalid card - id: %d", __FUNCTION__, cardIndex);
    }
    // process targetList
    const auto targetType = info.targetType;
    QVector<Entity *> targetList = {};
    switch (targetType) {
    case CardInfo::SELF:
        targetList.push_back(_entities[_playerSlot]);
        break;
    case CardInfo::ONE:
        qDebug() << "target_index:" << targetIndex;
        targetList.push_back(_entities[targetIndex]);
        break;
    case CardInfo::ALL:
        for (int i = 1; i < _entities.size(); ++i) {
            targetList.push_back(_entities[i]);
        }
        break;
    case CardInfo::RANDOM:
        default_random_engine *e = new default_random_engine();
        targetList.push_back(_entities[(*e)() % (_entities.size() - 1) + 1]);
        delete e;
        break;
    }

    auto ctx = Context{};
    ctx.from = this->_entities[_playerSlot];
    ctx.to = targetList;
    for (auto &i : actList) {
        i->act(&ctx);
    }
    this->handleContext(&ctx);

    for (auto iter = this->_handCard.begin(); iter != this->_handCard.end(); ++iter) {
        if ((*iter) == cardID) {
            iter = this->_handCard.erase(iter);
            break;
        }
    }
    this->_stack[DROP]->push({cardID});
}



void InGameSystem::handleCardValid(QString cardID, int *valid)
{
    const auto info = CardSystem::getCardInfo(cardID);
    *valid = !(info.actType & this->_actionDisabled);
}
