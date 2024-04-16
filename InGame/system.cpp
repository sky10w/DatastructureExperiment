#include "system.h"

System::System()
{
    _entities[0] = new Player();
    std::default_random_engine e;
    e.seed(std::time(nullptr));

    this->_enemyNum = e() % 3 + 1;
    for(int i = 0; i < this->_enemyNum; ++i)
    {
        _entities.push_back(new Enemy());
    }
}

void System::playerUsingCard(const QString &cardID)
{

}

// Round end for player's round
// Need to execute the following enemies' action
void System::roundEnd()
{
    for(int i = 1; i <= _enemyNum; ++i)
    {
        _entities[i]->roundBegin();

        _entities[i]->roundEnd();
    }
}

void System::needCardStack(bool isDrawStack)
{
    const auto allcards = _stack[isDrawStack]->getAll();
    emit sendCardStackInfo(allcards);
}

void System::needHandCard()
{
    emit sendHandCard(this->_handCard);
}
