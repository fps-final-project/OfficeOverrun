#include "pch.h"
#include "EnemyBuilder.hpp"
#include "Pathfinder.h"

EnemyBuilder& EnemyBuilder::WithNewEnemy(std::shared_ptr<AnimatedModel> model)
{
	m_enemy.reset();
	m_enemy = std::make_shared<Enemy>(model);
	return *this;
}

EnemyBuilder& EnemyBuilder::WithSound(std::shared_ptr<AudioFile> file, IXAudio2* xaudio)
{
	m_enemy->m_sound = std::make_shared<SourceVoice>(file, xaudio);
	return *this;
}

EnemyBuilder& EnemyBuilder::WithDamageSound(std::shared_ptr<AudioFile> file, IXAudio2* xaudio)
{
	m_enemy->m_damageSound = std::make_shared<SourceVoice>(file, xaudio);
	m_enemy->m_damageSound->SetVolume(10.0f);

	return *this;
}

EnemyBuilder& EnemyBuilder::WithHealth(int maxHealth)
{
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
	m_enemy->m_emitter->Position = { pos.x, pos.y, pos.z };
	return *this;
}

EnemyBuilder& EnemyBuilder::WithRotation(DirectX::XMFLOAT3 rot)
{
	m_enemy->rotation = rot;
	m_enemy->m_emitter->OrientFront = { rot.x, rot.y, rot.z };
	return *this;
}

EnemyBuilder& EnemyBuilder::WithVelocity(DirectX::XMFLOAT3 vel)
{
	m_enemy->velocity = vel;
	m_enemy->m_emitter->Velocity = { vel.x, vel.y, vel.z };
	return *this;
}

EnemyBuilder& EnemyBuilder::WithAttackRadius(float radius)
{
	m_enemy->radius = radius;
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

EnemyBuilder& EnemyBuilder::WithPath(std::shared_ptr<Pathfinder> pathfinder)
{
	m_enemy->pathToPlayer = pathfinder->FindPath(m_enemy->position);
	return *this;
}

std::shared_ptr<Enemy>& EnemyBuilder::Build()
{
    return m_enemy;
}
