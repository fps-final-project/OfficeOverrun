#include "pch.h"
#include "Enemy.hpp"
#include "Pathfinder.h"

std::vector<X3DAUDIO_DISTANCE_CURVE_POINT> Enemy::s_soundCurvePoints = {
		{ 2.0f,	0.9f },
		{ 3.0f, 0.7f },
		{ 4.0f, 0.5f },
		{ 5.0f, 0.3f },
};

X3DAUDIO_DISTANCE_CURVE Enemy::s_soundCurve = { s_soundCurvePoints.data(), s_soundCurvePoints.size() };

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
	m_emitter->pVolumeCurve = &s_soundCurve;

}

Action Enemy::Update(float dt, std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos)
{
	pathfinder->UpdatePath(pathToPlayer, position);
	Action currentAction;
	if (!isIdle() || !pathToPlayer.playerVisible) return currentAction;

	XMVECTOR direction = XMVector3Normalize(GetDirection());
	XMVECTOR playerDir = { playerPos.x - position.x, 0.f, playerPos.z - position.z };

	float l = XMVector3Length(playerDir).m128_f32[0];

	float dx = XMVectorGetX(direction);
	float dz = XMVectorGetZ(direction);

	float yaw = atan2(dx, dz);
	targetRotation = AdjustAngleToPositive(yaw);
	

	if (l <= radius)
	{
		int id = (rand() % 2) + 1;
		setAnimation("attack" + std::to_string(id), 1.5);
		currentAction = Action{ ActionType::ATTACK, {damage} };
	}


	XMFLOAT3 changedPos = XMFLOAT3{
		position.x + direction.m128_f32[0] * speed * dt,
		position.y,
		position.z + direction.m128_f32[2] * speed * dt,
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

void Enemy::takeDamage(int damage)
{
	health -= damage; 
	if (health <= 0) 
	{
		this->setAnimation(rand() % 2 == 0 ? "death" : "death2");
		this->m_animator.clearFallbackAnimation();
	}
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
	// fallback
	if (pathToPlayer.path.empty())
		return {1.f, 0.f, 0.f};

	return {
		pathToPlayer.path.front().position.x - position.x,
		pathToPlayer.path.front().position.y - position.y,
		pathToPlayer.path.front().position.z - position.z
	};
}
