#include "GameCard.h"

// CardStack

CardStack::CardStack(const QVector<QString> &cards) {
  auto seq = this->utilGetRandomNum(cards.size());
  for (auto i : seq) {
    this->_list.push_back(cards[i]);
  }
}

QString CardStack::getPopOne() {
  if (this->empty()) {
    return "-1";
  }
  const QString temp = this->_list.back();
  this->_list.pop_back();
  return temp;
}

QVector<QString> CardStack::getPopN(int n) {
  if (this->empty()) {
    return {"-1"};
  }
  QVector<QString> tempList;
  for (int i = 0; i<n &&this->_list.size()> 0; ++i) {
    const QString temp = this->_list.front();
    this->_list.pop_front();
    tempList.push_back(temp);
  }
  return tempList;
}

QVector<QString> CardStack::getPopAll() {
  if (this->empty()) {
    return {"-1"};
  }
  QVector<QString> tempList;
  while (this->_list.size() > 0) {
    const QString temp = this->_list.front();
    this->_list.pop_front();
    tempList.push_back(temp);
  }
  return tempList;
}

QVector<QString> CardStack::getAll() const {
  QVector<QString> tempList;
  for (auto &i : this->_list) {
    tempList.push_back(i);
  }

  return tempList;
}

void CardStack::push(const QVector<QString> &cards) {
  for (auto &i : cards) {
    this->_list.push_back(i);
  }
}

void CardStack::clear() { this->_list.clear(); }

bool CardStack::empty() const { return this->_list.empty(); }

int CardStack::size() const { return this->_list.size(); }

QVector<int> CardStack::utilGetRandomNum(int n)
{
    QVector<int> tempList;
    for (int i = 0; i < n; ++i)
        tempList.append(i);
    QVector<int> resList;
    for (size_t i = 0; i < n - 1; i++) {
        const int randInd = std::rand() % (n - i - 1);
        resList.push_back(tempList[randInd]);
        tempList[randInd] = tempList[n - i - 1];
    }
    return resList;
}
