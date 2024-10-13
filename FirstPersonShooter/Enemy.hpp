#pragma once
#include "AnimatedEntity.hpp"

class Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedAssimpModel> model);

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
};

