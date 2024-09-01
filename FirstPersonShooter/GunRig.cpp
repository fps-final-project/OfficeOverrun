#include "pch.h"
#include "GunRig.h"

GunRig::GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset)
{
	// idea - they can be shared with world.entites
	m_hands = std::make_shared<AnimatedEntity>(hands);
	m_gun = std::make_shared<AnimatedEntity>(gun, gunOffset);

	m_hands->setFallbackAnimation("FP_idle_pose");
	m_gun->setFallbackAnimation("GUN_idle_pose");
}

void GunRig::update(float dt)
{
	this->m_hands->Update(dt);
	this->m_gun->Update(dt);
}

void GunRig::reload()
{
	this->m_hands->setAnimation("FP_reload");
	this->m_gun->setAnimation("GUN_reload");
}

void GunRig::shoot()
{
	this->m_hands->setAnimation("FP_fire");
 	this->m_gun->setAnimation("GUN_fire");
}

std::vector<std::shared_ptr<AnimatedEntity>> GunRig::getEntites()
{
	return { m_hands, m_gun };
}

