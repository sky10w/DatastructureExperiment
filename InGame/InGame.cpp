#include "InGame.h"

InGame::InGame(const QVector<Entity*>& enemy)
{
	this->_player = new Player();
    this->_enemies = enemy;
}

void InGame::run()
{
	this->_player->RoundBegin();
	this->_player->RoundMid();
	this->_player->RoundEnd();

	for (auto entity : this->_enemies)
	{
		entity->RoundBegin();
		entity->RoundMid();
		entity->RoundEnd();
	}
}
