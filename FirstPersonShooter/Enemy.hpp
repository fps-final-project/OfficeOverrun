#pragma once
#include "AnimatedEntity.hpp"
#include "ActionTypes.hpp"
#include "Room.hpp"
#include "Path.h"
#include <list> 

class Pathfinder;

class  __declspec(dllexport) Enemy : public AnimatedEntity
{
public:
	Enemy(std::shared_ptr<AnimatedModel> model);
	Action Update(std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos);
	virtual void Update(float dt) override;
	Path pathToPlayer;

	friend class EnemyBuilder;
private:
	int maxHealth;
	int health;
	int damage;
	float speed;
	float radius;

	const float rotationSpeed = 4.f;
	float targetRotation;

	float GetRotationIncrement();
	float AdjustAngleToPositive(float angle);
	XMVECTOR GetDirection();
};

