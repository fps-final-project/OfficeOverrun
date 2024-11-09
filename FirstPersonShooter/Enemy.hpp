#pragma once
#include "AnimatedEntity.hpp"

class Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);
	void Update(XMFLOAT3 playerPosition);

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
	float speed;
	float radius;
};

