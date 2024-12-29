#include "pch.h"
#include "Enemy.hpp"
#include "Pathfinder.h"

Enemy::Enemy(std::shared_ptr<AnimatedModel> model) : AnimatedEntity{ model }, targetRotation(0.f)
{
	m_emitter = std::make_shared<X3DAUDIO_EMITTER>();
	m_emitter->Position = { position.x, position.y, position.z };
	m_emitter->OrientFront = { rotation.x, rotation.y, rotation.z };
	m_emitter->OrientTop = { 0.0f, 1.0f, 0.0f };
	m_emitter->Velocity = { 0.0f, 0.0f, 0.0f };
	m_emitter->pCone = nullptr;
	m_emitter->ChannelCount = 1;
	m_emitter->CurveDistanceScaler = 1.0f;
	m_emitter->DopplerScaler = 1.0f;
	
}

Action Enemy::Update(std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos)
{
	pathfinder->UpdatePath(pathToPlayer, position);
	Action currentAction;
	XMVECTOR direction = GetDirection();
	XMVECTOR playerDir = { playerPos.x - position.x, 0.f, playerPos.z - position.z };

	float l = XMVector3Length(playerDir).m128_f32[0];
	direction = XMVector3Normalize(direction);

	float dx = XMVectorGetX(direction);
	float dz = XMVectorGetZ(direction);

	float yaw = atan2(dx, dz);
	targetRotation = AdjustAngleToPositive(yaw);
	
	if (!isIdle() || !pathToPlayer.playerVisible) return currentAction;

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
	m_emitter->Position = { position.x, position.y, position.z };
	return currentAction;
}

void Enemy::Update(float dt)
{
	rotation.y = AdjustAngleToPositive(rotation.y + GetRotationIncrement() * dt);
	setRotation({ 0.0f, rotation.y, 0.0f });
	m_emitter->OrientFront = { rotation.x, rotation.y, rotation.z };
	if (pathToPlayer.playerVisible)
		AnimatedEntity::Update(dt);
}

float Enemy::GetRotationIncrement()
{
	float direction1 = targetRotation - rotation.y;
	int target = targetRotation;
	if (targetRotation > rotation.y)
		targetRotation -= DirectX::XM_2PI;
	else targetRotation += DirectX::XM_2PI;
	float direction2 = targetRotation - rotation.y;
	if (std::abs(direction1) < std::abs(direction2))
	{
		return direction1 * rotationSpeed;
	}
	else return direction2 * rotationSpeed;
}

float Enemy::AdjustAngleToPositive(float angle)
{
	return angle < 0.f ? angle + DirectX::XM_2PI : angle > DirectX::XM_2PI ? angle - DirectX::XM_2PI : angle;
}

XMVECTOR Enemy::GetDirection()
{
	return {
		pathToPlayer.path.front().position.x - position.x,
		pathToPlayer.path.front().position.y - position.y,
		pathToPlayer.path.front().position.z - position.z
	};
}
