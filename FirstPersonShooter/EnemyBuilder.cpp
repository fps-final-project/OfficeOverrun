#include "pch.h"
#include "EnemyBuilder.hpp"

EnemyBuilder& EnemyBuilder::WithNewEnemy(std::shared_ptr<AnimatedAssimpModel> model)
{
	m_enemy.reset();
	m_enemy = std::make_shared<Enemy>(model);
	return *this;
}

EnemyBuilder& EnemyBuilder::WithMaxHealth(int maxHealth)
{
	m_enemy->maxHealth = maxHealth;
	m_enemy->health = maxHealth;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithDamage(int health)
{
	m_enemy->damage = health;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithSpeed(float speed)
{
	m_enemy->speed = speed;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithPosition(DirectX::XMFLOAT3 pos)
{
	m_enemy->position = pos;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithRotation(DirectX::XMFLOAT3 rot)
{
	m_enemy->rotation = rot;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithVelocity(DirectX::XMFLOAT3 vel)
{
	m_enemy->velocity = vel;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithSize(DirectX::XMFLOAT3 size)
{
	m_enemy->size = size;
	return *this;
}

EnemyBuilder& EnemyBuilder::WithFallbackAnimation(std::string animationName)
{
	m_enemy->setFallbackAnimation(animationName);
	return *this;

}

std::shared_ptr<Enemy>& EnemyBuilder::Build()
{
    return m_enemy;
}
