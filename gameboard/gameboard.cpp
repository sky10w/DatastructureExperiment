#include "gameboard.h"
// QMap<QString, QString> MP_description = {{"1", "这是易伤BUFF"},
//                                         {"2", "111111S"}};
QMap<QString, QPixmap> CardIcon;
map<int, QString> ActionName = {{1 << 0, "ATTACK"},
                                {1 << 1, "DEFEND"},
                                {1 << 2, "BUFF"},
                                {1 << 3, "ACTION"},
                                {-1, "ERROR"}};
const int HANDSLIMIT = 7;
const int WIDGETW = 1280;
const int WIDGETH = 720;
const int FLASH_INTERVALS = 75;
const int FLASH_TIMES = 10;
int mysignbit(const qreal &x) { return (x > 0) - (x < 0); }
gameboard::gameboard(QWidget *parent)
    : MyOpenGLWidget(parent), Player(nullptr), Enemy(), myhands(nullptr),
      drawpile(nullptr), discardpile(nullptr)

{
    MyDebug;
    init();
}
void EntityView::initasplayer(int id)
{
    setPos(100, 200);
    setPixmap(QPixmap("://res/player.png"));
    mybuff.init(100, 200);
    this->type = "player";
    this->id = id;
    // name = name;
}
void EntityView::initasenemy(int id)
{
    setPos(300 + 250 * id, 200);
    setPixmap(QPixmap("://res/enemy.png"));
    mybuff.init(300 + 250 * id, 200);
    this->id = id;
    this->type = "enemy";

    action.setParentItem(this);
    action.setPixmap(QString("://res/action.jpg") /*现在还没有action icon */);
    action.setPos((width - action.pixmap().width()) / 2, -action.pixmap().height());
    // name = name;
}

void EntityView::updatehpview()
{
    hpnumber.setText(QString::number(HP));
    hpbar.setLine(posx, posy, (qreal) HP / HP_MAX * width * 0.9, posy);
}

void EntityView::update_HP(int delta)
{
    HP += delta;
    if (HP <= 0) {
        HP = 0;
        this->hide();
        this->mybuff.buffview.hide();
    } else if (HP > HP_MAX)
        HP = HP_MAX;
    updatehpview();
}
void EntityView::updatearmorview()
{
    armornumber.setText(QString::number(armor));
    if (armor > 0)
        armoricon.show();
    else
        armoricon.hide();
}

void EntityView::update_buff(QString id, int delta)
{
    mybuff.update_buff(id, delta);
}
void EntityView::update_armor(int delta)
{
    armor += delta;
    updatearmorview();
}

void EntityView::update_action(Action::Act_t id) {
  this->action.setPixmap(QString("://res/" + ActionName[id] + ".png"));
}

void gameboard::updateaction(map<int /*enemy_id*/, Action::Act_t> mp) {
  for (auto x : mp)
    Enemy[x.first]->update_action(x.second);
}

void gameboard::act(int id, Action::Act_t action) {}

void gameboard::updateenergyview() {
  EnergyButton.setText(QString(u8"当前能量为:") + QString::number(energy));
}

void gameboard::init() {
  MyDebug << "Init";
  scene.clear();
  view.setScene(nullptr);
  if (Player != nullptr)
    delete Player;
  for (auto x : Enemy)
    if (x.second != nullptr) {
      delete x.second;
    }
  Enemy.clear();
  if (myhands != nullptr)
    delete myhands;
  if (drawpile != nullptr)
    delete drawpile;
  if (discardpile != nullptr)
    delete discardpile;
  this->myhands = new HandsView();
  this->myhands->handsview = &view;
  this->myhands->handsscene = &scene;
  this->myhands->setParent(this);
  this->myhands->init();
  this->Player = nullptr;

  drawpile = new DrawPileView();
  discardpile = new DiscardPileView();
  discardpile->init();
  drawpile->init();
  view.setParent(this);
  view.setFixedSize(WIDGETW, WIDGETH);
  view.setScene(&scene);
  scene.setParent(this);
  scene.setSceneRect(0, 0, WIDGETW, WIDGETH);
  scene.setItemIndexMethod(QGraphicsScene::NoIndex);

  this->background.setPixmap(QString("://res/background.jpg"));
  scene.addItem(&background);
  background.setPos(0, 0);

  buttons = {&DiscardPileButton, &DrawPileButton, &EndButton, &SettingsButton,
             &EnergyButton};
  DiscardPileButton.setParent(this);
  DrawPileButton.setParent(this);
  SettingsButton.setParent(this);
  EnergyButton.setParent(this);
  EndButton.setParent(this);
  DiscardPileButton.resize(80, 80);
  DrawPileButton.resize(80, 80);
  SettingsButton.resize(50, 50);
  EnergyButton.resize(80, 80);
  EndButton.resize(100, 50);

  DiscardPileButton.move(1200, 640);
  DrawPileButton.move(0, 640);
  SettingsButton.move(1200, 50);
  EnergyButton.move(130, 550);
  EndButton.move(1000, 500);
  connect(drawpile, &DrawPileView::send_card_to_hands, myhands,
          &HandsView::carddraw);
  connect(&EndButton, &QPushButton::pressed, [=]() {
    playerround = 0;
    emit roundover();
  });
  connect(&DiscardPileButton, &QPushButton::pressed, [=]() {
    if (view.scene() == discardpile) {
      for (auto x : buttons)
        x->show();
      if (Player != nullptr)
        Player->mybuff.buffview.show();
      for (auto x : Enemy)
        x.second->mybuff.buffview.show();
      view.setScene(&scene);

    } else {
      for (auto x : buttons) {
        if (x != &DiscardPileButton)
          x->hide();
      }
      if (Player != nullptr)
        Player->mybuff.buffview.hide();
      for (auto x : Enemy)
        x.second->mybuff.buffview.hide();
      view.setScene(discardpile);
    }
  });
  connect(&DrawPileButton, &QPushButton::pressed, [=]() {
    if (view.scene() == drawpile) {
      for (auto x : buttons)
        x->show();
      if (Player != nullptr)
        Player->mybuff.buffview.show();
      for (auto x : Enemy)
        x.second->mybuff.buffview.show();
      view.setScene(&scene);

    } else {
      for (auto x : buttons) {
        if (x != &DrawPileButton)
          x->hide();
      }
      if (Player != nullptr)
        Player->mybuff.buffview.hide();
      for (auto x : Enemy)
        x.second->mybuff.buffview.hide();
      view.setScene(drawpile);
    }
  });
  connect(&EnergyButton, &QPushButton::pressed, this, [=]() {
    // for (QString x : {"0001", "0002"})
    //  updatebuff(x, 2, Enemy[1]->id);
    // shuffle();//
    // QString x("6 6 6");
    // QString y = x.split(" ");
    // cout << y << Endl;
  });
  connect(&SettingsButton, &QPushButton::pressed, [=]() {});
  connect(myhands, &HandsView::discardcard, this, &gameboard::discardcard);
  connect(drawpile, &DrawPileView::shuffle, this, &gameboard::shuffle);

  view.setStyleSheet("background: transparent;border:0px");

  DiscardPileButton.setText("弃牌堆");
  DrawPileButton.setText("摸牌堆");
  EndButton.setText("结束回合");
  EnergyButton.setText("当前能量为:?");
  SettingsButton.setText("设置");

  // for (int i = 0; i < 5; i++)
  //  drawpile->addcard("1");
  view.show();
}
void EntityView::init(int hp)
{
    MyDebug;
    HP = hp;
    HP_MAX = hp;
    hpbar.setParentItem(this);
    hpbar.setPen(QPen(Qt::red, 10 /*线宽*/));
    hpbar.setLine(0, 0, width * 0.9, 0);
    hpbar.setZValue(0);
    hpbar.setPos(0, width);

    hpnumber.setText(QString::number(hp));
    hpnumber.setParentItem(&hpbar);
    hpnumber.setZValue(1);
    hpnumber.setPos(0, -10);
    hpnumber.setFont(QFont("Arial", 15, 0, true));
    hpnumber.setBrush(Qt::white);

    armoricon.setParentItem(this);
    armoricon.setPos(0.8 * width, 0.8 * width);
    armoricon.setPixmap(QPixmap("://res/armor.png"));

    armornumber.setParentItem(&armoricon);
    armornumber.setBrush(Qt::white);
    armornumber.setPos(25, 20);
    armornumber.setText(QString::number(armor));
    armornumber.setFont(QFont("Arial", 15, 0, true));
}

void gameboard::roundbegin()
{
    MyDebug << "Round begin";
    playerround = 1;
    QPixmap msg(QString("://res/roundbegin.png"));
    setanimation(msg, 3000, WIDGETW - msg.width() >> 1, WIDGETH - msg.height() >> 1);
}
void gameboard::setanimation(QPixmap pixmap, int time, qreal posx, qreal posy) {
  QGraphicsPixmapItem *msg = new QGraphicsPixmapItem(pixmap);
  scene.addItem(msg);
  msg->setPos(posx, posy);
  QTimer *timer = new QTimer(); //新建一个
  timer->setSingleShot(true);   //一次性
  QObject::connect(timer, &QTimer::timeout, this, [=] {
    scene.removeItem(msg);
    delete msg;
  });
  timer->start(time);
  //---
}
void BuffView::init(int posx, int posy)
{
    buffview.setScene(&buffscene);
    buffview.setGeometry(posx, posy + 260 + 10, 260, 50);
    buffview.setFixedSize(260, 60);
    buffview.setStyleSheet("background:transparent;border: 0px");
    buffview.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    buffview.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    buffview.horizontalScrollBar()->setStyleSheet("QScrollBar {height:10px;}");
    buffview.show();
}

void BuffView::update_buff(QString id, int delta)
{
    cout << id.toStdString() << ' ' << delta << endl;
    auto it = Buffs.find(id);
    auto it2 = bufficon.find(id);
    if (it != Buffs.end()) {
        it->second->strength += delta;
        if (it->second->strength <= 0) {
            it->second->strength = 0;
            this->buffscene.removeItem(it2->second);
            delete it->second;
            delete it2->second;
            Buffs.erase(it);
            bufficon.erase(it2);
            return;
        }
        it2->second->setToolTip(QString(
            "<div style='background-color: #87CEEB; color: white; padding: "
            "0px; border: none;'>"
            "<p style='font-family: \"微软雅黑\", sans-serif; font-size: "
            "12px;'>"
            + it->second->description + "持续回合数:" + QString::number(it->second->strength)
            + "</p>"
              "</div>"));
    } else {
        // cout << id.toStdString() << endl;
        if (delta <= 0)
            return;
        Buff *newbuff = new Buff(id, delta);
        BuffIcon *newbufficon = new BuffIcon();

        // newbufficon->setPixmap("://res/" + id + ".png");
        newbufficon->setPixmap(newbuff->iconpath);
        // cout << newbuff->iconpath.toStdString() << endl;
        newbufficon->description = newbuff->description;

        newbufficon->setToolTip(
            QString("<div style='background-color: #87CEEB; color: white; padding: "
                    "0px; border: none;'>"
                    "<p style='font-family: \"微软雅黑\", sans-serif; font-size: "
                    "12px;'>"
                    + newbufficon->description + "持续回合数:" + QString::number(newbuff->strength)
                    + "</p>"
                      "</div>"));

        buffscene.addItem(newbufficon);

        // cout << Buffs.size() << endl;
        Buffs[id] = newbuff;
        bufficon[id] = newbufficon;
        // cout << Buffs.size() << endl;
    }

    updateview();
}

Buff::Buff(QString uuid, int strength) {
  auto it = BuffSystem::getBuffInfo(uuid);
  this->description = it.description;
  this->iconpath = it.iconPath;
  this->uuid = uuid;
  this->strength = strength;
}
BuffView::BuffView() { buffview.setSceneRect(0, 0, 260, 60); }
void BuffView::updateview() {
  int k = 0;
  for (auto x : bufficon)
    x.second->setPos(50 * (k++), 0);
  buffview.show();
}
void CardView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
  EntityView *itembelow;
  itembelow = dynamic_cast<EntityView *>(
      scene()->itemAt(event->scenePos(), QTransform()));

  HandsView *hands = dynamic_cast<HandsView *>(this->parent());
  gameboard *w = dynamic_cast<gameboard *>(hands->parent());
  // cout << endl;
  // for (auto x : w->Enemy)
  //  cout << x.first << ' ';
  // cout << endl;
  arrow.hide();
  int *valid = new int();
  emit w->request_valid(uuid, valid);

  auto distance = QLineF(curposx + 120, curposy + 120, event->scenePos().x(),
                         event->scenePos().y())
                      .length();
  auto deny = [this]() -> void {
    setPos(curposx, curposy);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setZValue(0);
  };
  if (distance < 200)
    deny();
  else if (!*valid)
    deny();
  else if (w->energy < info.energy)
    deny();
  else if (info.targetType == info.ONE &&
           (itembelow == nullptr || itembelow->type != "enemy"))
    deny();
  else if (w->playerround == 0)
    deny();
  else {
    cout << this->info.name.toStdString() << endl;
    inhands = false;
    // cout << itembelow->id << endl;
    if (itembelow != nullptr) {
      cout << itembelow->id << endl;
      emit playcard(itembelow->id);
    } else {
      emit playcard(0);
    }
  }
  hands = nullptr;
  w = nullptr;
  itembelow = nullptr;
}
void CardView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    qreal distance
        = QLineF(curposx + 75, curposy + 125, event->scenePos().x(), event->scenePos().y()).length();
    EntityView *itembelow = dynamic_cast<EntityView *>(
        scene()->itemAt(event->scenePos(), QTransform()));
    HandsView *hands = dynamic_cast<HandsView *>(this->parent());
    gameboard *w = dynamic_cast<gameboard *>(hands->parent());
    if (distance > 200) {
        setFlag(QGraphicsItem::ItemIsMovable, false);
        //更新现有箭头的位置
        arrow.show();
        if (info.targetType == info.ONE && (itembelow == nullptr || itembelow->type == "player"))
            arrow.setPen(QPen(Qt::red, 2));
        else if (w->energy < info.energy)
            arrow.setPen(QPen(Qt::red, 2));
        else
            arrow.setPen(QPen(Qt::green, 2));
        arrow.setLine(75,
                      125,
                      event->pos().x() - 5 * mysignbit(event->pos().x()),
                      event->pos().y() - 5 * mysignbit(event->pos().y()));

    } else {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        arrow.hide();
    }
    itembelow = nullptr;
}
void CardView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    this->setZValue(1);
}
void CardView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    if (this->inhands) {
        this->setZValue(1);
        this->setPos(this->pos().x(), this->pos().y() - 50);
    }
}
void CardView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    if (this->inhands) {
        this->setZValue(0);
        this->setPos(curposx, curposy);
    }
}
void CardView::init()
{
    info = CardSystem::getCardInfo(this->uuid);
    setPixmap(QPixmap(info.path));
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
    // info = cards[uuid];

    // valid = 1;
    arrow.setParentItem(this);
}
// void CardView::mouse
void HandsView::init()
{
    handsscene->setItemIndexMethod(QGraphicsScene::NoIndex);
}
void HandsView::carddraw(CardView *x)
{
    if (hands.size() >= HANDSLIMIT) {
        emit discardcard(x);
        return;
    }
    handsscene->addItem(x);
    x->setParent(this);
    x->inhands = true;
    x->setFlag(QGraphicsItem::ItemIsMovable, true);
    x->setAcceptHoverEvents(true);
    connect(x, &CardView::playcard, this, &HandsView::consumecard);
    hands.push_front(x);
    updatecard();
}
void HandsView::updatecard()
{
    for (int i = 0; i < hands.size(); i++) {
        hands[i]->curposx = 1200 - 150 * (hands.size() - i);
        hands[i]->curposy = 550;
        hands[i]->setPos(hands[i]->curposx, hands[i]->curposy);
    }
}
void HandsView::consumecard(int id)
{
    MyDebug << "Consume a card - index" << id;
    CardView *trashcard = nullptr;
    int index;
    for (int i = 0; i < hands.size(); i++)
        if (hands[i]->inhands == false) {
            trashcard = hands[i];
            // cout << i << endl;
            index = i;
            break;
        }
    // cout << hands.size() << endl;
    // cout << index << endl;
    if (trashcard == nullptr)
        return;
    hands.remove(index);
    handsscene->removeItem(trashcard);
    discardcard(trashcard);

    emit playcard(index, id);
    updatecard();
}
void gameboard::discardcard(CardView *card)
{
    MyDebug << "Discard a card - id" << card->uuid;
    discardpile->addcard(card);

    /// card = nullptr;
}

void gameboard::updateenergy(int delta)
{
    energy += delta;
    updateenergyview();
}

void gameboard::setenergy(int x) {
  energy = x;
  updateenergyview();
}

void gameboard::updatebuff(QString buffuuid, int strength, int id) {
  if (id != 0)
    Enemy[id]->update_buff(buffuuid, strength);
  else
    Player->update_buff(buffuuid, strength);
}

void gameboard::updatehp(int id, int delta)
{
    if (id != 0) {
        Enemy[id]->update_HP(delta);
        if (delta < 0 && Enemy[id]->HP > 0) {
            AnimationHurt(Enemy[id]);
        }

    } else {
        Player->update_HP(delta);
        if (delta < 0 && Player->HP > 0)
            AnimationHurt(Player);
    }
}
void gameboard::AnimationHurt(EntityView *entity) {
  for (int i = 0; i < FLASH_TIMES; i++) {
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, [=]() {
      if (i % 2 == 0)
        entity->hide();
      else
        entity->show();
    });
    timer->start(FLASH_INTERVALS * i);
  }
  QPixmap gethurt(QString("://res/gethurt.png"));
  setanimation(gethurt, FLASH_INTERVALS * FLASH_TIMES,
               entity->pos().x() +
                   (entity->pixmap().size().width() - gethurt.width()) / 2,
               entity->pos().y() +
                   (entity->pixmap().size().height() - gethurt.height()) / 2);
}
// void gameboard::AnimationDefend(EntityView *entity) {}
// void gameboard::AnimationFortify(EntityView *entity) {}
// void gameboard::Animation()

void gameboard::updatearmor(int id, int delta) {
  if (id != 0)
    Enemy[id]->update_armor(delta);
  else
    Player->update_armor(delta);
}

void gameboard::initenemy(int id, QString name, int HP_MAX) {
  MyDebug << "Init enemy - id" << id;
  EntityView *newenemy = new EntityView();
  scene.addItem(newenemy);
  newenemy->name = name;
  newenemy->init(HP_MAX);
  newenemy->initasenemy(id);
  newenemy->setParent(this);
  newenemy->mybuff.buffview.setParent(this);
  Enemy[id] = newenemy;
}
void gameboard::initplayer(int id /*, QString name,*/, int HP_MAX) {
  MyDebug << "Init player";
  // Player->HP_MAX = HP_MAX;
  EntityView *newplayer = new EntityView();
  scene.addItem(newplayer);
  newplayer->init(HP_MAX);
  newplayer->initasplayer(id);
  newplayer->setParent(this);
  newplayer->mybuff.buffview.setParent(this);
  Player = newplayer;
}
void gameboard::shuffle() {
  MyDebug << "Shuffle once";
  // drawpile->st.insert(discardpile->st.begin(), discardpile->st.end());
  for (std::pair<QString, CardView *> x : discardpile->st) {
    /// *Test code*
    if (x.second->scene() == nullptr)
      continue;
    discardpile->removeItem(x.second);
    drawpile->st.insert(x);
    // drawpile->st.insert(x.first, x.second);
  }

  discardpile->clean();
  for (auto &[x, y] : drawpile->st) {
    drawpile->addItem(y);
  }
  // for (auto &p : drawpile->st) {
  // drawpile->addItem(p);
  // }
  drawpile->update();
}
void DiscardPileView::addcard(CardView *card) {
  MyDebug << "add card - id" << card->uuid;
  addItem(card);
  card->inhands = 0;
  card->setFlag(QGraphicsItem::ItemIsMovable, false);
  auto it = card->uuid;
  st.insert({it, card});
  // card = nullptr;
  update();
}
void DiscardPileView::init() {
  setItemIndexMethod(NoIndex);
  description.setPixmap(QPixmap("://res/1713154927636415.png"));
  description.setZValue(1);
  description.setPos(475, 0);
  background.setPixmap(QPixmap("://res/background.png"));
  background.setZValue(0);
  background.setPos(0, 0);
  addItem(&description);
  addItem(&background);
  setSceneRect(0, 0, WIDGETW, WIDGETH);
}
void DiscardPileView::clean() {
  MyDebug << "Discardpile clean";
  for (auto &x : st)
    removeItem(x.second);
  st.clear();
}
void DiscardPileView::update() {
  MyDebug << "Discardpile update - size" << st.size();
  int i = 0, j = 0;
  for (auto &x : st) {
    x.second->setPos(265 + i * 150, 100 + j * 250);
    x.second->curposx = 265 + i * 150;
    x.second->curposy = 100 + j * 250;
    i++;
    if (i == 5)
      i = 0, j++;
  }
}
void DrawPileView::update() {
  MyDebug << "Drawpile update - size" << st.size();
  MyDebug << "now st contains:";
  for (auto &i : st) {
    qDebug() << "               " << i.first;
  }
  qDebug();
  int i = 0, j = 0;
  for (auto &x : st) {
    x.second->setPos(265 + i * 150, 100 + j * 250);
    x.second->curposx = 265 + i * 150;
    x.second->curposy = 100 + j * 250;
    // x->setPos(265 + i * 150, 100 + j * 250);
    // x->curposx = 265 + i * 150;
    // x->curposy = 100 + j * 250;
    i++;
    if (i == 5)
      i = 0, j++;
  }
}
void DrawPileView::init() {
  MyDebug << "Init";
  setItemIndexMethod(NoIndex);
  description.setPixmap(QPixmap("://res/1713155278729455.png"));
  description.setZValue(1);
  description.setPos(475, 0);
  background.setPixmap(QPixmap("://res/background.png"));
  background.setZValue(0);
  background.setPos(0, 0);
  addItem(&description);
  addItem(&background);
  setSceneRect(0, 0, WIDGETW, WIDGETH);
  /// *Test code*
  MyDebug << "Clear stack";
  st.clear();

  MyDebug << "now st contains:";
  for (auto &i : st) {
    qDebug() << "               " << i.first;
  }
  qDebug();
}
void DrawPileView::drawcard(QString uuid) {
  MyDebug << "Draw a card - id" << uuid;
  if (st.size() == 0)
    emit shuffle(); //没牌就洗一下
  if (st.size() == 0)
    return; //还没牌就返回

  if (st.find(uuid) == st.end()) {
    qFatal("No Card Found in Drawpile - uuid: %s", uuid.toLatin1().data());
  }
  // CardView *x = *st.find(uuid);
  CardView *x = st.find(uuid)->second;
  removecard(uuid);
  emit send_card_to_hands(x);
  // x = nullptr;
}
void DrawPileView::addcard(QString uuid) {
  CardView *newcard = new CardView();
  st.insert({uuid, newcard});
  // st.insert(uuid, newcard);
  MyDebug << "now st contains:";
  for (auto &i : st) {
    qDebug() << "               " << i.first;
  }
  qDebug();
  newcard->uuid = uuid;
  newcard->init();
  // std::cout << newcard->info.path.toStdString() << endl;
  addItem(newcard);
  update();
  newcard = nullptr;
}
void DrawPileView::removecard(QString uuid) {
  auto it = st.find(uuid);
  // removeItem(*it);
  removeItem(it->second);
  st.erase(it);
  update();
}
