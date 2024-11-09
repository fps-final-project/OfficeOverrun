#include "pch.h"
#include "Enemy.hpp"

Enemy::Enemy(std::shared_ptr<AnimatedModel> model) : AnimatedEntity{model}
{
}

void Enemy::Move(XMFLOAT3 playerPosition)
{
	XMVECTOR direction = {
		playerPosition.x - position.x,
		playerPosition.y - 1 - position.y,
		playerPosition.z - position.z
	};

	direction = XMVector3Normalize(direction);

	XMFLOAT3 changedPos = XMFLOAT3{
		position.x + direction.m128_f32[0] * speed,
		position.y + direction.m128_f32[1] * speed,
		position.z + direction.m128_f32[2] * speed,
	};

	setPosition(changedPos);
}
