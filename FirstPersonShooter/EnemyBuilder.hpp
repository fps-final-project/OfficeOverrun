#pragma once
#include "Enemy.hpp"
#include <memory>

class EnemyBuilder
{
public:
	EnemyBuilder& WithNewEnemy(std::shared_ptr<AnimatedModel> model);
	EnemyBuilder& WithMaxHealth(int maxHealth);
	EnemyBuilder& WithDamage(int health);
	EnemyBuilder& WithSpeed(float speed);
	EnemyBuilder& WithPosition(DirectX::XMFLOAT3 pos);
	EnemyBuilder& WithRotation(DirectX::XMFLOAT3 rot);
	EnemyBuilder& WithVelocity(DirectX::XMFLOAT3 vel);
	EnemyBuilder& WithAttackRadius(float radius);
	EnemyBuilder& WithSize(DirectX::XMFLOAT3 size);
	EnemyBuilder& WithFallbackAnimation(std::string animationName);
	std::shared_ptr<Enemy>& Build();
private:
	std::shared_ptr<Enemy> m_enemy;
};

