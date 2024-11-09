#include "pch.h"
#include "Enemy.hpp"

Enemy::Enemy(std::shared_ptr<AnimatedModel> model) : AnimatedEntity{model}
{
}

void Enemy::Update(XMFLOAT3 playerPosition)
{

	XMVECTOR direction = {
		playerPosition.x - position.x,
		playerPosition.y - 1 - position.y,
		playerPosition.z - position.z
	};
	float l = XMVector3Length(direction).m128_f32[0];
	direction = XMVector3Normalize(direction);

	float dx = XMVectorGetX(direction);
	float dz = XMVectorGetZ(direction);

	float yaw = atan2(dx, dz);

	setRotation({0.0f, yaw, 0.0f});
	
	if (!isIdle()) return;
	
	if (l <= radius)
	{
		int id = (rand() % 2) + 1;
		setAnimation( "attack" + std::to_string(id), 1.5);
	}


	XMFLOAT3 changedPos = XMFLOAT3{
		position.x + direction.m128_f32[0] * speed,
		position.y + direction.m128_f32[1] * speed,
		position.z + direction.m128_f32[2] * speed,
	};

	setPosition(changedPos);
}
