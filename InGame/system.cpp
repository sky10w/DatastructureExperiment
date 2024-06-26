#include "system.h"

QVector<QString> GlobalStatus::allCardOwned = {
    "0001", "0001", "0001", "0002", "0003", "0003", "0003", "0004", "0005"};
int GlobalStatus::playerMaxHp = 40;
int GlobalStatus::playerMaxEnergy = 10;
int GlobalStatus::playerMaxHandCard = 7;

const int InGameSystem::_playerSlot = 0;
const int InGameSystem::_animation_duration = 1000;
InGameSystem::InGameSystem(QWidget *parent)
    : QWidget(parent)
    , _playerEnergy(0)
    , _gView(nullptr)
    , _scene(nullptr)
    , _view(nullptr)
{
    this->_inited = false;
    this->setFixedSize(1280, 720);
    this->_stack[0] = new CardStack();
    this->_stack[1] = new CardStack();

    // this->_view = new gameboard();
    // connectSignalSlotForView();

    _actionDisabled = 0;
    _handCard = {};
}

void InGameSystem::initSystem(bool isBossLevel)
{
    if (_inited)
        return;

    MyDebug << "executing init InGameSystem";
    // Init view
    this->_view = new gameboard();
    connectSignalSlotForView();

    // Init scene
    this->_scene = new QGraphicsScene();
    _scene->addWidget(_view);
    _gView = new QGraphicsView(this);
    _gView->setFrameShape(QFrame::NoFrame);
    _gView->setFrameRect({0, 0, 0, 0});
    _gView->setScene(_scene);
    // _gView->hide();

    // Initialize
    _inited = true;
    _actionDisabled = 0;
    _handCard = {};
    _gameover = 0;
    _playerEnergy = GlobalStatus::playerMaxEnergy;

    // Init CardStack
    for (auto &i : _stack) {
        i->clear();
    }

    // Init player
    auto player = new Player(InGameSystem::_playerSlot, 40);
    _entities.push_back(player);
    connectSignalSlotForEntities(player);
    _view->initplayer(InGameSystem::_playerSlot, GlobalStatus::playerMaxHp);
    _enemyStrategy.push_back(nullptr);

    // Init enemies
    if (isBossLevel == false) {
        MyDebug << "Normal Level";
        this->_enemyNum = GlobalRandomEngine::getRandom() % 3 + 1;

        for (int i = 1; i <= this->_enemyNum; ++i) {
            _entities.push_back(new NormalEnemy(i, 20));
            _enemyStrategy.push_back(new NormalStrategy());
            connectSignalSlotForEntities(_entities[i]);
            this->_view->initenemy(i, "://res/enemy.jpg", 20);
        }
    } else {
        MyDebug << "Boss Level";
        this->_enemyNum = 1;
        _entities.push_back(new Boss(1, 30));
        _enemyStrategy.push_back(new BossStrategy());
        connectSignalSlotForEntities(_entities[1]);
        this->_view->initenemy(1, "://res/enemy.jpg", 30);
    }
    _gView->show();
}

void InGameSystem::gameend(bool isWin)
{
    MyDebug << "Gameend";
    _inited = false;
    _actionDisabled = 0;
    for (auto &i : _entities) {
        disconnectSignalSlotForEntities(i);
        i->deleteLater();
    }
    _entities.clear();
    _enemyNum = 0;
    _curEntity = 0;
    _playerEnergy = 0;
    for (auto &i : this->_stack) {
        i->clear();
    }
    disconnectSignalSlotForView();
    _view->deleteLater();
    _view = nullptr;
    _scene->deleteLater();
    _scene = nullptr;
    _gView->deleteLater();
    _gView = nullptr;
    _handCard.clear();

    emit gameover(isWin);
}

void InGameSystem::run()
{
    MyDebug << "Run the game";
    // Init cardStack
    auto list = GlobalStatus::allCardOwned;
    int len = list.size();
    for (int i = len - 1; i >= 0; --i) {
        int ind = GlobalRandomEngine::getRandom() % (i + 1);
        swap(list[ind], list[i]);
    }
    this->_stack[DROP]->clear();
    this->_stack[DRAW]->push(list);
    for (auto &i : list) {
        MyDebug << "Add Card To Stack - id:" << i;
        emit addCardToStack(i);
    }

    // Init handCard
    for (int i = 0; i < 5; ++i) {
        const auto res = drawCard();
        if (res == false)
            break;
    }

    this->_actionDisabled = 0;
    _playerEnergy = GlobalStatus::playerMaxEnergy;
    emit setEnergy(GlobalStatus::playerMaxEnergy);

    this->generateNextRoundHint();
    this->_entities[0]->roundBegin();
    emit playerRoundBegin();
}

// Round end for player's round
// Need to execute the following enemies' action
void InGameSystem::playerRoundEnd()
{
    this->_entities[0]->roundEnd();

    /// Enemy round
    this->enemyRound();

    /// Player round
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [=] {
        this->_curEntity = 0;

        if (this->_stack[DRAW]->empty()) {
            this->shuffle();
        }
        for (int i = 0; i < 2; ++i) {
            const auto res = drawCard();
            if (res == false)
                break;
        }
        _playerEnergy = GlobalStatus::playerMaxEnergy;
        emit setEnergy(GlobalStatus::playerMaxEnergy);
        this->_actionDisabled = 0;

        this->_entities[0]->roundBegin();

        /// Generate Next Round
        this->generateNextRoundHint();
        emit playerRoundBegin();
    });
    timer->start((this->_enemyNum) * _animation_duration);
    // timer->deleteLater();
}

void InGameSystem::enemyRound()
{
    MyDebug << "Enemy Round: enemyNum:" << this->_enemyNum;

    for (int i = 1; i <= _enemyNum; ++i) {
        MyDebug << "Round End: now enemy:" << i;
        if (_entities[i]->isDead())
            continue;

        QTimer *timer = new QTimer();
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, [=] {
            _curEntity = i;
            _entities[i]->roundBegin();

            /// Test
            auto cur = dynamic_cast<NormalEnemy *>(this->_entities[i]);
            auto actCtx = cur->getNextAction();
            emit this->entityAct(i, actCtx.first);
            this->handleContext(actCtx.second);

            _entities[i]->roundEnd();
        });
        timer->start((i - 1) * _animation_duration);
        // timer->deleteLater();
    }
}

void InGameSystem::shuffle()
{
    MyDebug << "Shuffle one time";
    emit this->sendShuffle();
    const auto list = this->_stack[DROP]->getPopAll();
    this->_stack[DRAW]->push(list);
}

bool InGameSystem::drawCard()
{
    if (this->_handCard.size() >= 7)
        return false;
    const auto cardID = this->_stack[DRAW]->getPopOne();
    if (cardID == "-1") {
        MyDebug << "Fail to draw a card";
        return false;
    }
    MyDebug << "Player: Draw a card";
    MyDebug << "After draw stack";
    _stack[DRAW]->print();
    _handCard.push_front(cardID);
    emit addCardToHand(cardID);
    return true;
}

void InGameSystem::generateNextRoundHint()
{
    // Generate next round hint
    std::map<int, Action::Act_t> actMap;
    for (int i = 1; i <= _enemyNum; ++i) {
        NormalEnemy *cur = dynamic_cast<NormalEnemy *>(this->_entities[i]);
        Context *ctx = new Context();
        ctx->from = cur;
        ctx->to = {this->_entities[_playerSlot]};
        const auto act = this->_enemyStrategy[i]->generateAction(ctx);
        cur->setNextAction({act, ctx});
        actMap.insert({i, act});
    }
    emit nextRoundHint(actMap);
}

int InGameSystem::checkGameover()
{
    bool flag = true;
    for (int i = 1; i < this->_entities.size(); ++i) {
        if (!this->_entities[i]->isDead()) {
            flag = false;
            break;
        }
    }
    if (flag == true)
        return 1;
    if (this->_entities[0]->isDead())
        return 2;
    return 0;
}

void InGameSystem::handleContext(Context *ctx)
{
    if (ctx->damageDone != 0) {
        ctx->from->attack(ctx, true);
    }
    if (ctx->armorGained != 0) {
        for (auto i : ctx->to) {
            i->gainArmor(ctx, true);
        }
    }
    if (ctx->buffGiven != "") {
        // auto &str = ctx->buffGiven;
        ctx->from->giveBuff(ctx, true);
    }
    if (ctx->buffRemoved != "") {
        // auto &str = ctx->buffRemoved;
        ctx->from->removeBuff(ctx);
    }
    if (ctx->actAltered != 0) {
        this->_actionDisabled ^= ctx->actAltered;
    }
    if (ctx->hpHealed != 0) {
        for (auto i : ctx->to) {
            i->heal(ctx);
        }
    }

    MyDebug << "Set gameover";
    _gameover = checkGameover();
}

void InGameSystem::playerUsingCard(int cardIndex, int targetIndex)
{
    MyDebug << "Player using card - index:" << cardIndex;
    const auto cardID = this->_handCard[cardIndex];
    this->_handCard.erase(next(this->_handCard.begin(), cardIndex));
    this->_stack[DROP]->push({cardID});

    MyDebug << "                  - id:" << cardID;
    const auto info = CardSystem::getCardInfo(cardID);
    if (this->_playerEnergy < info.energy) {
        qFatal("In function %s: Unable to use card - cardID: %s - No energy - "
               "current Energy: %d",
               __FUNCTION__,
               info.id.toLatin1().data(),
               this->_playerEnergy);
    }
    this->_playerEnergy -= info.energy;
    emit this->updateEnergy(-info.energy);

    const auto actList = info.action;
    if (targetIndex >= _entities.size()) {
        qFatal("In function %s: Unable to access to entity - index: %d", __FUNCTION__, targetIndex);
    }
    const auto target = _entities[targetIndex];
    if (target->isDead()) {
        qFatal("In function %s: Unable to target a dead entity - index: %d",
               __FUNCTION__,
               targetIndex);
    }
    const auto actType = info.actType;
    if ((actType & this->_actionDisabled) > 0) {
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
        MyDebug << "target_index:" << targetIndex;
        targetList.push_back(_entities[targetIndex]);
        break;
    case CardInfo::ALL:
        for (int i = 1; i < _entities.size(); ++i) {
            targetList.push_back(_entities[i]);
        }
        break;
    case CardInfo::RANDOM:
        targetList.push_back(
            _entities[GlobalRandomEngine::getRandom() % (_entities.size() - 1) + 1]);
        break;
    }

    auto ctx = Context{};
    ctx.from = this->_entities[_playerSlot];
    ctx.to = targetList;
    for (auto &i : actList) {
        i->act(&ctx);
    }
    this->handleContext(&ctx);

    if (_gameover) {
        gameend(_gameover == 1);
        return;
    }
}

void InGameSystem::handleCardValid(QString cardID, int *valid)
{
    const auto info = CardSystem::getCardInfo(cardID);
    *valid = !(info.actType & this->_actionDisabled);
}

void InGameSystem::connectSignalSlotForEntities(Entity *entity)
{
    MyDebug << "Connect To Entity once";
    QObject::connect(entity,
                     SIGNAL(requestHandleContext(Context *)),
                     this,
                     SLOT(handleContext(Context *)));
    QObject::connect(entity, SIGNAL(hpChanged(int, int)), _view, SLOT(updatehp(int, int)));
    QObject::connect(entity, SIGNAL(armorChanged(int, int)), _view, SLOT(updatearmor(int, int)));
    QObject::connect(entity,
                     SIGNAL(buffChanged(QString, int, int)),
                     _view,
                     SLOT(updatebuff(QString, int, int)));
}

void InGameSystem::disconnectSignalSlotForEntities(Entity *entity)
{
    MyDebug << "Disconnect To Entity once";
    QObject::connect(entity,
                     SIGNAL(requestHandleContext(Context *)),
                     this,
                     SLOT(handleContext(Context *)));
    QObject::disconnect(entity, SIGNAL(hpChanged(int, int)), _view, SLOT(updatehp(int, int)));
    QObject::disconnect(entity, SIGNAL(armorChanged(int, int)), _view, SLOT(updatearmor(int, int)));
    QObject::disconnect(entity,
                        SIGNAL(buffChanged(QString, int, int)),
                        _view,
                        SLOT(updatebuff(QString, int, int)));
}

void InGameSystem::connectSignalSlotForView()
{
    MyDebug << "Connect To View once";
    QObject::connect(_view, SIGNAL(roundover()), this, SLOT(playerRoundEnd()));
    QObject::connect(_view,
                     SIGNAL(request_valid(QString, int *)),
                     this,
                     SLOT(handleCardValid(QString, int *)));
    QObject::connect(this, SIGNAL(updateEnergy(int)), _view, SLOT(updateenergy(int)));
    QObject::connect(this, SIGNAL(addCardToStack(QString)), _view->drawpile, SLOT(addcard(QString)));
    QObject::connect(this, SIGNAL(addCardToHand(QString)), _view->drawpile, SLOT(drawcard(QString)));
    QObject::connect(this, SIGNAL(playerRoundBegin()), _view, SLOT(roundbegin()));
    QObject::connect(this, SIGNAL(sendShuffle()), _view, SLOT(shuffle()));
    QObject::connect(this, SIGNAL(setEnergy(int)), _view, SLOT(setenergy(int)));
    QObject::connect(_view->myhands,
                     SIGNAL(playcard(int, int)),
                     this,
                     SLOT(playerUsingCard(int, int)));
    QObject::connect(this,
                     SIGNAL(nextRoundHint(Action::ActMap_t)),
                     _view,
                     SLOT(updateaction(Action::ActMap_t)));
    QObject::connect(this,
                     SIGNAL(entityAct(int, Action::Act_t)),
                     _view,
                     SLOT(act(int, Action::Act_t)));
}

void InGameSystem::disconnectSignalSlotForView()
{
    MyDebug << "Disconnect To View once";
    QObject::disconnect(_view, SIGNAL(roundover()), this, SLOT(playerRoundEnd()));
    QObject::disconnect(_view,
                        SIGNAL(request_valid(QString, int *)),
                        this,
                        SLOT(handleCardValid(QString, int *)));
    QObject::disconnect(this, SIGNAL(updateEnergy(int)), _view, SLOT(updateenergy(int)));
    QObject::disconnect(this,
                        SIGNAL(addCardToStack(QString)),
                        _view->drawpile,
                        SLOT(addcard(QString)));
    QObject::disconnect(this,
                        SIGNAL(addCardToHand(QString)),
                        _view->drawpile,
                        SLOT(drawcard(QString)));
    QObject::disconnect(this, SIGNAL(playerRoundBegin()), _view, SLOT(roundbegin()));
    QObject::disconnect(this, SIGNAL(sendShuffle()), _view, SLOT(shuffle()));
    QObject::disconnect(this, SIGNAL(setEnergy(int)), _view, SLOT(setenergy(int)));
    QObject::disconnect(_view->myhands,
                        SIGNAL(playcard(int, int)),
                        this,
                        SLOT(playerUsingCard(int, int)));
    QObject::disconnect(this,
                        SIGNAL(nextRoundHint(Action::ActMap_t)),
                        _view,
                        SLOT(updateaction(Action::ActMap_t)));
    QObject::disconnect(this,
                        SIGNAL(entityAct(int, Action::Act_t)),
                        _view,
                        SLOT(act(int, Action::Act_t)));
}
