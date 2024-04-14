#include "system.h"

System::System()
    : _player(new Player)
{
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
