#include "pch.h"
#include "Enemy.hpp"
#include "Pathfinder.h"

Enemy::Enemy(std::shared_ptr<AnimatedModel> model) : AnimatedEntity{ model }
{
}

Action Enemy::Update(std::shared_ptr<Pathfinder> pathfinder)
{
	pathfinder->UpdatePath(pathToPlayer, position);
	Action currentAction;
	XMVECTOR direction = GetDirection();

	float l = XMVector3Length(direction).m128_f32[0];
	direction = XMVector3Normalize(direction);

	float dx = XMVectorGetX(direction);
	float dz = XMVectorGetZ(direction);

	float yaw = atan2(dx, dz);

	setRotation({ 0.0f, yaw, 0.0f });

	if (!isIdle()) return currentAction;

	if (l <= radius)
	{
		int id = (rand() % 2) + 1;
		setAnimation("attack" + std::to_string(id), 1.5);
		currentAction = Action{ ActionType::ATTACK, {damage} };
	}


	XMFLOAT3 changedPos = XMFLOAT3{
		position.x + direction.m128_f32[0] * speed,
		position.y,
		position.z + direction.m128_f32[2] * speed,
	};

	setPosition(changedPos);
	return currentAction;
}

XMVECTOR Enemy::GetDirection()
{
	if (pathToPlayer.size() == 1)
	{
		return {
			pathToPlayer.front().position.x - position.x,
			pathToPlayer.front().position.y - position.y,
			pathToPlayer.front().position.z - position.z
		};
	}
	auto secondLastNode = (++pathToPlayer.rbegin());
	return {
		secondLastNode->position.x - position.x,
		secondLastNode->position.y - position.y,
		secondLastNode->position.z - position.z
	};

}
