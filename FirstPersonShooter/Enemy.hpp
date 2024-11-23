#pragma once
#include "AnimatedEntity.hpp"
#include "ActionTypes.hpp"
#include "Room.hpp"
#include "PathNodeData.h"
#include <list> 

class Pathfinder;

class Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);
	Action Update(std::shared_ptr<Pathfinder> pathfinder);
	std::list<PathNodeData> pathToPlayer;

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
	float speed;
	float radius;

	XMVECTOR GetDirection();
};

