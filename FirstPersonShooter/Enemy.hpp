#pragma once
#include "AnimatedEntity.hpp"
#include "ActionTypes.hpp"
#include "Room.hpp"
#include "PathNodeData.h"
#include "Path.h"
#include <list> 

class Pathfinder;


class Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);
	Action Update(std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos);
	Path pathToPlayer;

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
	float speed;
	float radius;

	XMVECTOR GetDirection();
};

