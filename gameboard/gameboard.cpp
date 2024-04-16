#include "gameboard.h"
vector<pair<int, QString>> MP_description = {{1, u8"这是易伤BUFF"}};
QMap<QString, QPixmap> CardIcon;
QMap<QString, CardInfo> cards = {{"1", {"1", "name", "1", "1", 1, 1}}};
const int HANDSLIMIT = 7;
const int WIDGETW = 1280;
const int WIDGETH = 720;
int mysignbit(const qreal &x) { return (x > 0) - (x < 0); }
gameboard::gameboard(MyOpenGLWidget *parent) : MyOpenGLWidget{parent} {

  // QGraphicsScene scene(this);
  // QGraphicsView view(this);
  // this->setStyleSheet("background:white");
  Player = new EntityView();
  auto enemy = new EntityView();
  Enemy.push_back(enemy);
  energy = 15;
  updateenergyview();
  myhands = new HandsView();
  buttons = {&DiscardPileButton, &DrawPileButton, &EndButton, &SettingsButton,
             &EnergyButton};

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
  scene.addItem(Player);
  scene.addItem(Enemy[0]);

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
  connect(&EndButton, &QPushButton::pressed, [=]() { QApplication::quit(); });
  connect(&DiscardPileButton, &QPushButton::pressed, [=]() {
    if (view.scene() == discardpile) {
      for (auto x : buttons)
        x->show();
      Player->mybuff.buffview.show();
      for (auto x : Enemy)
        x->mybuff.buffview.show();
      view.setScene(&scene);

    } else {
      for (auto x : buttons) {
        if (x != &DiscardPileButton)
          x->hide();
      }
      Player->mybuff.buffview.hide();
      for (auto x : Enemy)
        x->mybuff.buffview.hide();
      view.setScene(discardpile);
    }
    // view.show();
  });
  connect(&DrawPileButton, &QPushButton::pressed, [=]() {
    if (view.scene() == drawpile) {
      for (auto x : buttons)
        x->show();
      Player->mybuff.buffview.show();
      for (auto x : Enemy)
        x->mybuff.buffview.show();
      view.setScene(&scene);

    } else {
      for (auto x : buttons) {
        if (x != &DrawPileButton)
          x->hide();
      }
      Player->mybuff.buffview.hide();
      for (auto x : Enemy)
        x->mybuff.buffview.hide();
      view.setScene(drawpile);
    }
    // view.show();
  });
  connect(&EnergyButton, &QPushButton::pressed, this, [=]() {
    drawpile->drawcard("1");
    // myhands->que.push_back(te)
  });
  connect(&SettingsButton, &QPushButton::pressed, [=]() { this->shuffle(); });
  connect(myhands, &HandsView::discardcard, this, &gameboard::discardcard);
  connect(drawpile, &DrawPileView::shuffle, this, &gameboard::shuffle);

  view.setStyleSheet("background: transparent;border:0px");

  DiscardPileButton.setText("弃牌堆");
  DrawPileButton.setText("摸牌堆");
  EndButton.setText("结束回合");
  EnergyButton.setText("当前能量为:?");
  SettingsButton.setText("设置");

  view.setAutoFillBackground(false);

  Player->init(100);
  Player->initasplayer();
  Player->mybuff.buffview.setParent(this);
  Player->parent = this;

  Enemy[0]->init(100);
  Enemy[0]->initasenemy();
  Enemy[0]->mybuff.buffview.setParent(this);
  Enemy[0]->parent = this;

  myhands->handsview = &view;
  myhands->handsscene = &scene;
  myhands->setParent(this);
  myhands->init();
  // myhands->init();
  // myhands->test = new CardView();
  for (int i = 0; i < 5; i++)
    drawpile->addcard("1");
  // myhands->carddraw();
  // CardView *testcard = new CardView();
  // delete testcard;
  // testcard->init();
  // CardView *testcard2 = new CardView();
  // testcard2->init();
  // delete testcard;
  // delete testcard2;

  // myhands->que.push_back(testcard);

  // myhands->que.push_back(testcard2);

  // myhands->carddraw();

  view.show();
}
void EntityView::init(int hp) {
  HP = hp;
  // int width = 260;

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
// QRectF EntityView::boundingRect() const { return QRectF(0, 0, width, height);
// }
void EntityView::initasplayer() {
  setPos(100, 200);
  setPixmap(QPixmap("://res/player.png"));
  mybuff.init(100, 200);
}
void EntityView::initasenemy() {
  setPos(700, 200);
  setPixmap(QPixmap("://res/enemy.jpg"));
  mybuff.init(700, 200);
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

void EntityView::updatebuff(int id, int delta) { mybuff.updatebuff(id, delta); }
void EntityView::update_armor(int delta) {
  armor += delta;
  updatearmorview();
}

void EntityView::update_action(int id) {}

void gameboard::updateenergyview() {
  EnergyButton.setText(QString(u8"当前能量为:") + QString::number(energy));
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

void BuffView::updatebuff(int id, int delta) {
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
    Buffs[id] = newbuff;
    bufficon[id]->strength.setText(QString::number(delta));
    newbuff = nullptr;
  }
  int k = 0;
  for (auto x : bufficon) {
    x.second->setPos(50 * (k++), 0);
  }
}
Buff::Buff(int uuid, int strength) {
  this->uuid = uuid;
  this->strength = strength;
}
BuffView::BuffView() {
  // getBuff();
  MAXBUFFSUM = 1;
  for (int i = 0; i < MAXBUFFSUM; i++) {
    BuffIcon *x = new BuffIcon();

    x->setPixmap(
        QPixmap("://res/" + QString::number(MP_description[i].first) + ".png"));
    x->setAcceptHoverEvents(true);
    x->description = MP_description[i].second;
    x->setToolTip(x->description);
    bufficon[MP_description[i].first] = x;
    buffscene.addItem(x);
    x->show();
    x = nullptr;
  }
}
void BuffView::updateview() {}
// void BuffIcon::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {}
// void BuffIcon::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {}
// CardView::~CardView() { delete this; }
void CardView::get_valid(bool isvalid) { valid = isvalid; }
void CardView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

  QGraphicsItem::mouseReleaseEvent(event);
  EntityView *itembelow;
  itembelow = dynamic_cast<EntityView *>(
      scene()->itemAt(event->scenePos(), QTransform()));
  // energy = 1;

  HandsView *hands = dynamic_cast<HandsView *>(this->parent());
  gameboard *w = dynamic_cast<gameboard *>(hands->parent());
  arrow.hide();

  emit request_valid(cardtype);

  auto distance = QLineF(curposx + 120, curposy + 120, event->scenePos().x(),
                         event->scenePos().y())
                      .length();

  auto deny = [this]() -> void {
    setPos(curposx, curposy);
    setFlag(QGraphicsItem::ItemIsMovable, true);
  };
  if (distance < 200)
    deny();
  else if (!valid)
    deny();
  else if (w->energy < info.energy)
    deny();
  else if (itembelow == nullptr)
    deny();
  else if (cardtype == 1 && itembelow->type == "enemy")
    deny();
  else {
    w->energy -= info.energy;
    w->updateenergyview();
    // cout << info.energy << ' ' << w->energy << endl;
    inhands = 0;

    emit playcard(itembelow->id);
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
  // cout << ' ' << curposx + 75 << ' ' << curposy + 125 << endl;
  // cout << ' ' << event->scenePos().x() << ' ' << event->scenePos().y() <<
  // endl; cout << distance << endl;
  EntityView *itembelow;
  itembelow = dynamic_cast<EntityView *>(
      scene()->itemAt(event->scenePos(), QTransform()));
  // if (itembelow != nullptr) {
  // cout << itembelow->x() << ' ' << itembelow->y() << ' ' << endl;
  // cout << itembelow->HP << endl;
  //}
  if (distance > 200) {
    setFlag(QGraphicsItem::ItemIsMovable, false);
    //更新现有箭头的位置
    arrow.show();
    // arrow.setLine(0, 0, 100, 100);
    arrow.setLine(
        75, 125, event->pos().x() - 5 * mysignbit(event->pos().x()),
        event->pos().y() -
            5 * mysignbit(
                    event->pos().y())); // event->pos.x(), event->pos.y());
    arrow.setPen(QPen(Qt::red, 2));
  } else {
    setFlag(QGraphicsItem::ItemIsMovable, true);
    arrow.hide();
  }
  itembelow = nullptr;
}
void CardView::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  arrow.show();
}
void CardView::init() {
  setPixmap(QPixmap("://res/card.jpg"));
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
  info = cards[uuid];
  valid = 1;
  arrow.setParentItem(this);
  // this->setFlag(QGraphicsItem::ItemIsSelectable, false);
  // arrow.set
}
// CardInfo::CardInfo()
//    : description(""), name(""), uuid("1"), type(0), consumption(0) {}
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
  hands.push_front(x);

  connect(hands[0], &CardView::playcard, this, &HandsView::consumecard);
  updatecard();
}

void HandsView::updatecard() {

  for (int i = 0; i < hands.size(); i++) {
    hands[i]->curposx = 1000 - 150 * i;
    hands[i]->curposy = 550;
    hands[i]->setPos(hands[i]->curposx, hands[i]->curposy);
  }
}
void HandsView::consumecard(int id) {

  CardView *trashcard;
  for (int i = 0; i < hands.size(); i++)
    if (hands[i]->inhands == false) {
      trashcard = hands[i];
      break;
    }
  if (trashcard == nullptr)
    return;
  // cout << hands.indexOf(trashcard) << endl;
  hands.remove(hands.indexOf(trashcard));
  // cout << hands.size() << endl;

  handsscene->removeItem(trashcard);
  discardcard(trashcard);

  trashcard = nullptr;
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

void gameboard::updatebuff(int buffid, int strength, int objectid) {
  if (objectid != 0)
    Enemy[objectid]->updatebuff(buffid, strength);
  else
    Player->updatebuff(buffid, strength);
}

void gameboard::updatehp(int objectid, int delta) {
  if (objectid != 0)
    Enemy[objectid]->update_HP(delta);
  else
    Player->update_HP(delta);
}

void gameboard::updatearmor(int objectid, int delta) {
  if (objectid != 0)
    Enemy[objectid]->update_armor(delta);
  else
    Player->update_armor(delta);
}
void gameboard::shuffle() {
  // drawpile->st.insert(discardpile->st.begin(), discardpile->st.end());
  for (auto x : discardpile->st)
    drawpile->st.insert(x);
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
