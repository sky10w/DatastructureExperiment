#include "gameboard.h"
QMap<QString, QString> MP_description = {{"1", "这是易伤BUFF"}};
QMap<QString, QPixmap> CardIcon;
// QMap<QString, CardInfo> cards = {{"1", {"1", "name", "1", "1", 1, 1}}};
const int HANDSLIMIT = 7;
const int WIDGETW = 1280;
const int WIDGETH = 720;
int mysignbit(const qreal &x) { return (x > 0) - (x < 0); }
gameboard::gameboard(MyOpenGLWidget *parent) : MyOpenGLWidget{parent} {

  // QGraphicsScene scene(this);
  // QGraphicsView view(this);
  // this->setStyleSheet("background:white");
  // Player = new EntityView();

  // energy = 15;
  // updateenergyview();
  myhands = new HandsView();
  myhands->handsview = &view;
  myhands->handsscene = &scene;
  myhands->setParent(this);
  myhands->init();
  Player = nullptr;
  // initplayer(0, 100);
  // initenemy(1, 100);

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
  connect(&EnergyButton, &QPushButton::pressed, this, [=]() {});
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
void EntityView::init(int hp) {
  HP = hp;
  HP_MAX = hp;
  hpbar.setParentItem(this);
  hpbar.setPen(QPen(Qt::red, 10 /*线宽*/));
  hpbar.setLine(0, 0, width, 0);
  hpbar.setZValue(0);
  hpbar.setPos(0, width);

  hpnumber.setText(QString::number(hp));
  hpnumber.setParentItem(&hpbar);
  hpnumber.setZValue(1);
  hpnumber.setPos(width / 2 - 15, -5);
  hpnumber.setFont(QFont("Arial", 15, 0, true));

  armoricon.setParentItem(this);
  armoricon.setPos(0.8 * width, 0.8 * width);
  armoricon.setPixmap(QPixmap("://res/1.png"));

  armornumber.setParentItem(&armoricon);
  armornumber.setBrush(Qt::white);
  armornumber.setPos(20, 20);
  armornumber.setText(QString::number(armor));
  armornumber.setFont(QFont("Arial", 15, 0, true));
}
void EntityView::initasplayer(int id) {
  setPos(100, 200);
  setPixmap(QPixmap("://res/player.png"));
  mybuff.init(100, 200);
  id = id;
  // name = name;
}
void EntityView::initasenemy(int id) {
  setPos(500 + 200 * id, 200);
  setPixmap(QPixmap("://res/enemy.jpg"));
  mybuff.init(500 + 200 * id, 200);
  id = id;

  action.setParentItem(this);
  action.setPixmap(QString("://res/action.jpg") /*现在还没有action icon */);
  action.setPos((width - action.pixmap().width()) / 2,
                -action.pixmap().height());
  // name = name;
}

void EntityView::updatehpview() {
  hpnumber.setText(QString::number(HP));
  hpbar.setLine(posx, posy, (qreal)HP / HP_MAX * width, posy);
}

void EntityView::update_HP(int delta) {
  HP += delta;
  if (HP > HP_MAX)
    HP = HP_MAX;
  updatehpview();
}
void EntityView::updatearmorview() {
  armornumber.setText(QString::number(armor));
  if (armor > 0)
    armoricon.show();
  else
    armoricon.hide();
}

void EntityView::update_buff(QString id, int delta) {
  mybuff.update_buff(id, delta);
}
void EntityView::update_armor(int delta) {
  armor += delta;
  updatearmorview();
}

void EntityView::update_action(int actionid) {
  action.setPixmap(QString(";//"));
}

void gameboard::updateenergyview() {
  EnergyButton.setText(QString(u8"当前能量为:") + QString::number(energy));
}

void gameboard::roundbegin() {
  playerround = 1;
  QGraphicsPixmapItem *msg = new QGraphicsPixmapItem();
  msg->setPixmap(QString("://res/roundbegin.png"));
  scene.addItem(msg);
  msg->setPos(WIDGETW - msg->pixmap().width() >> 1,
              WIDGETH - msg->pixmap().height() >> 1);
  // 创建一个定时器
  QTimer *timer = new QTimer();
  timer->setSingleShot(true); // 设置定时器只触发一次
  // 连接定时器的timeout信号到一个lambda函数，该函数将删除item
  QObject::connect(timer, &QTimer::timeout, this, [=] {
    // 在场景中删除item
    scene.removeItem(msg);
    delete msg;
  });
  timer->start(3000);
}
void BuffView::init(int posx, int posy) {
  buffview.setScene(&buffscene);
  buffview.setGeometry(posx, posy + 260 + 10, 260, 50);
  buffview.setFixedSize(260, 60);
  buffview.setStyleSheet("background:transparent;border: 0px");
  buffview.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  buffview.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  buffview.horizontalScrollBar()->setStyleSheet("QScrollBar {height:10px;}");
  buffview.show();
}

void BuffView::update_buff(QString id, int delta) {
  auto it = Buffs.find(id);
  auto it2 = bufficon.find(id);
  if (it != Buffs.end()) {
    it->second->strength += delta;
    if (delta <= 0) {
      buffscene.removeItem(it2->second);
      bufficon.erase(id);
      Buffs.erase(id);
    }
  } else {
    Buff *newbuff = new Buff(id, delta);
    BuffIcon *newbufficon = new BuffIcon();
    buffscene.addItem(newbufficon);
    newbufficon->setPixmap("://res/" + id + ".png");
    newbufficon->description = MP_description[id];
    newbufficon->setToolTip(newbufficon->description);
    Buffs[id] = newbuff;
    bufficon[id] = newbufficon;
  }
  int k = 0;
  for (auto x : bufficon) {
    x.second->setPos(50 * (k++), 0);
  }
}
Buff::Buff(QString uuid, int strength) {
  this->uuid = uuid;
  this->strength = strength;
}
BuffView::BuffView() {}
void BuffView::updateview() {}
void CardView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mouseReleaseEvent(event);
  EntityView *itembelow;
  itembelow = dynamic_cast<EntityView *>(
      scene()->itemAt(event->scenePos(), QTransform()));
  HandsView *hands = dynamic_cast<HandsView *>(this->parent());
  gameboard *w = dynamic_cast<gameboard *>(hands->parent());
  arrow.hide();
  int *valid = 0;
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
  else if (itembelow == nullptr)
    deny();
  else if (cardtype == 1 && itembelow->type == "enemy")
    deny();
  else if (w->playerround == 0)
    deny();
  else {
    w->energy -= info.energy;
    w->updateenergyview();
    inhands = false;
    emit playcard(itembelow->name);
  }
  hands = nullptr;
  w = nullptr;
  itembelow = nullptr;
}
void CardView::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseMoveEvent(event);
  qreal distance = QLineF(curposx + 75, curposy + 125, event->scenePos().x(),
                          event->scenePos().y())
                       .length();
  EntityView *itembelow;
  itembelow = dynamic_cast<EntityView *>(
      scene()->itemAt(event->scenePos(), QTransform()));
  if (distance > 200) {
    setFlag(QGraphicsItem::ItemIsMovable, false);
    //更新现有箭头的位置
    arrow.show();
    arrow.setLine(75, 125, event->pos().x() - 5 * mysignbit(event->pos().x()),
                  event->pos().y() - 5 * mysignbit(event->pos().y()));
    arrow.setPen(QPen(Qt::red, 2));
  } else {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    arrow.hide();
  }
  itembelow = nullptr;
}
void CardView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  this->setZValue(1);
}
void CardView::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  QGraphicsItem::hoverEnterEvent(event);
  this->setZValue(1);
  this->setPos(this->pos().x(), this->pos().y() - 50);
}
void CardView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  QGraphicsItem::hoverLeaveEvent(event);
  this->setZValue(0);
  this->setPos(curposx, curposy);
}
void CardView::init() {
  setPixmap(QPixmap(info.path));
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
  // info = cards[uuid];
  info = CardSystem::getCardInfo(this->uuid);
  // valid = 1;
  arrow.setParentItem(this);
}
// void CardView::mouse
void HandsView::init() {
  handsscene->setItemIndexMethod(QGraphicsScene::NoIndex);
}
void HandsView::carddraw(CardView *x) {
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
void HandsView::updatecard() {
  for (int i = 0; i < hands.size(); i++) {
    hands[i]->curposx = 1000 - 150 * i;
    hands[i]->curposy = 550;
    hands[i]->setPos(hands[i]->curposx, hands[i]->curposy);
  }
}
void HandsView::consumecard(QString name) {

  CardView *trashcard = nullptr;
  int index;
  for (int i = 0; i < hands.size(); i++)
    if (hands[i]->inhands == false) {
      trashcard = hands[i];
      index = i;
      break;
    }
  if (trashcard == nullptr)
    return;
  hands.remove(index);
  handsscene->removeItem(trashcard);
  discardcard(trashcard);
  emit playcard(index, name);
  updatecard();
}
void gameboard::discardcard(CardView *card) {
  discardpile->addcard(card);
  /// card = nullptr;
}

void gameboard::updateenergy(int delta) {
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

void gameboard::updatehp(int id, int delta) {
  if (id != 0)
    Enemy[id]->update_HP(delta);
  else
    Player->update_HP(delta);
}

void gameboard::updatearmor(int id, int delta) {
  if (id != 0)
    Enemy[id]->update_armor(delta);
  else
    Player->update_armor(delta);
}

void gameboard::initenemy(int id /*QString name,*/, int HP_MAX) {
  EntityView *newenemy = new EntityView();
  scene.addItem(newenemy);
  newenemy->init(HP_MAX);
  newenemy->initasenemy(id);
  newenemy->setParent(this);
  newenemy->mybuff.buffview.setParent(this);
  Enemy[id] = newenemy;
}
void gameboard::initplayer(int id /*, QString name,*/, int HP_MAX) {
  // Player->HP_MAX = HP_MAX;
  EntityView *newplayer = new EntityView();
  scene.addItem(newplayer);
  newplayer->init(HP_MAX);
  newplayer->initasplayer(id);
  newplayer->setParent(this);
  newplayer->mybuff.buffview.setParent(this);
  Enemy[id] = newplayer;
  Player = newplayer;
  Player->mybuff.update_buff("1", 1);
}
void gameboard::shuffle() {
  // drawpile->st.insert(discardpile->st.begin(), discardpile->st.end());
  for (auto x : discardpile->st) {
    discardpile->removeItem(x.second);
    drawpile->st.insert(x);
  }

  discardpile->clean();
  for (auto &[x, y] : drawpile->st) {
    drawpile->addItem(y);
  }
  drawpile->update();
}
void DiscardPileView::addcard(CardView *card) {
  addItem(card);
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
  for (auto &x : st)
    removeItem(x.second);
  st.clear();
}
void DiscardPileView::update() {
  int i = 0, j = 0;
  for (auto &x : st) {
    x.second->setPos(265 + i * 150, 100 + j * 250);
    i++;
    if (i == 5)
      i = 0, j++;
  }
}
void DrawPileView::update() {
  int i = 0, j = 0;
  for (auto &x : st) {
    x.second->setPos(265 + i * 150, 100 + j * 250);
    i++;
    if (i == 5)
      i = 0, j++;
  }
}
void DrawPileView::init() {
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
}
void DrawPileView::drawcard(QString uuid) {
  if (st.size() == 0)
    emit shuffle(); //没牌就洗一下
  if (st.size() == 0)
    return; //还没牌就返回
  CardView *x = st.find(uuid)->second;
  removecard(uuid);
  emit send_card_to_hands(x);
  // x = nullptr;
}
void DrawPileView::addcard(QString uuid) {
  CardView *newcard = new CardView();
  st.insert({uuid, newcard});
  newcard->uuid = uuid;
  newcard->init();
  addItem(newcard);
  update();
  newcard = nullptr;
}
void DrawPileView::removecard(QString uuid) {
  auto it = st.find(uuid);
  removeItem(it->second);
  st.erase(it);
  update();
}
