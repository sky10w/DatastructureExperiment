#include "GameCard.h"

size_t qHash(CardInfo k, size_t seed) { return qHash(k.id, seed); }

size_t qHash(const CardInfo &k, size_t seed) { return qHash(k.id, seed); }
QHash<QString, CardInfo> CardSystem::cards;
QHash<QString, CardInfo> CardSystem::InitCards() {
  QFile file(opt::cardFilePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "Failed to open file " << opt::cardFilePath;
    return {};
  }
  QByteArray data = file.readAll();
  file.close();

  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &err);
  if (err.error != QJsonParseError::NoError) {
    qDebug() << "Error when parse to jsonDocument " << err.errorString();
    return {};
  }

  if (!doc.isArray()) {
    qWarning() << "Warning: Failed to parse the json into an json arry";
    return {};
  }

  QHash<QString, CardInfo> temp;
  QJsonArray array = doc.array();
  for (auto i : array) {
    if (!i.isObject()) {
      qWarning() << "Warning: invalid json format";
      return {};
    }
    QJsonObject obj = i.toObject();
    if (!obj.contains("uuid") || !obj.contains("name") ||
        !obj.contains("description") || !obj.contains("path") ||
        !obj.contains("target_type") || !obj.contains("energy")) {
      qWarning() << "Warning: invalid json format - "
                 << "a card doesn't have attribute \"uuid\"";
    }
    const QString id = obj["uuid"].toString();
    const QString name = obj["name"].toString();
    const QString description = obj["description"].toString();
    const QString path = obj["path"].toString();
    const int target_type = obj["target_type"].toInt();
    const int energy = obj["energy"].toInt();
    qDebug() << "Successfully load a card " << id << " -> " << path;
    temp[id] = {id, name, description, path, target_type, energy};
  }
  return temp;
}

CardSystem::CardSystem() {
  QFile file(opt::cardFilePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "Failed to open file " << opt::cardFilePath;
    return;
  }
  QByteArray data = file.readAll();
  file.close();

  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson(data, &err);
  if (err.error != QJsonParseError::NoError) {
    qDebug() << "Error when parse to jsonDocument " << err.errorString();
    return;
  }

  if (!doc.isArray()) {
    qWarning() << "Warning: Failed to parse the json into an json arry";
    return;
  }

  QJsonArray array = doc.array();
  for (auto i : array) {
    if (!i.isObject()) {
      qWarning() << "Warning: invalid json format";
      return;
    }
    QJsonObject obj = i.toObject();
    if (!obj.contains("uuid") || !obj.contains("name") ||
        !obj.contains("description") || !obj.contains("path") ||
        !obj.contains("target_type") || !obj.contains("energy")) {
      qWarning() << "Warning: invalid json format - "
                 << "a card doesn't have attribute \"uuid\"";
    }
    const QString id = obj["uuid"].toString();
    const QString name = obj["name"].toString();
    const QString description = obj["description"].toString();
    const QString path = obj["path"].toString();
    const int target_type = obj["target_type"].toInt();
    const int energy = obj["energy"].toInt();
    // this->cards[id] = {id, name, description, path, target_type, energy};
  }
}

CardStack::CardStack(const QVector<QString> &cards) {
  auto seq = this->utilGetRandomNum(cards.size());
  for (auto i : seq) {
    this->_list.push_back(cards[i]);
  }
}

QString CardStack::getPopOne() {
  const QString temp = this->_list.front();
  this->_list.pop_front();
  return temp;
}

QVector<QString> CardStack::getPopN(int n) {
  QVector<QString> tempList;
  for (int i = 0; i<n &&this->_list.size()> 0; ++i) {
    const QString temp = this->_list.front();
    this->_list.pop_front();
    tempList.push_back(temp);
  }
  return tempList;
}

QVector<QString> CardStack::getPopAll() {
  QVector<QString> tempList;
  while (this->_list.size() > 0) {
    const QString temp = this->_list.front();
    this->_list.pop_front();
    tempList.push_back(temp);
  }
  return tempList;
}

void CardStack::push(const QVector<QString> &cards) {
  for (auto &i : cards) {
    this->_list.push_back(i);
  }
}

bool CardStack::empty() const { return this->_list.empty(); }

QVector<int> CardStack::utilGetRandomNum(int n) {
  QVector<int> tempList;
  for (int i = 0; i < n; ++i)
    tempList.append(i);
  QVector<int> resList;
  for (size_t i = 0; i < n; i++) {
    const int randInd = std::rand() % (n - i - 1);
    resList.push_back(tempList[randInd]);
    tempList[randInd] = tempList[n - i - 1];
  }
  return resList;
}

CardInfo::CardInfo(const QString id, const QString name,
                   const QString description, const QString path,
                   int targetType, int energy) {
  this->id = id;
  this->name = name;
  this->description = description;
  this->targetType = targetType;
  this->energy = energy;
  this->path = path;
}
/*
CardView::CardView(const QString id, QWidget *parent)
    : QWidget(parent), _pressed(false), _id(id) {
  this->resize({180, 240});
  const QString path = CardSystem::cards[this->_id].path;
  qDebug() << "Picture path: " << path;
  this->setStyleSheet(QString{"background-image: url(:"} + path + ");");
}

void CardView::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void CardView::mousePressEvent(QMouseEvent *event) {
  _pressed = true;
  lastPoint = event->globalPos();
  initPoint = this->pos();
}

void CardView::mouseMoveEvent(QMouseEvent *event) {
  const auto pt = event->globalPos();
  this->move(pt - lastPoint + initPoint);
}

void CardView::mouseReleaseEvent(QMouseEvent *event) { _pressed = false; }
*/
