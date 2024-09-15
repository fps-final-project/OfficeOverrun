#include "pch.h"
#include "GunRig.h"

GunRig::GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset, DirectX::XMFLOAT3 barrelOffset)
	: m_gunOffset(gunOffset), m_initialBarrelOffset(barrelOffset), m_barrelOffset(barrelOffset)
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
	const float speedup = 1.6f;
	this->m_hands->setAnimation("FP_fire", speedup);
	this->m_gun->setAnimation("GUN_fire", speedup);
}

DirectX::XMFLOAT3 GunRig::getBarrelOffset()
{
	return m_barrelOffset;
}

std::vector<std::shared_ptr<AnimatedEntity>> GunRig::getEntites()
{
	return { m_hands, m_gun };
}

void GunRig::rotate(DirectX::XMFLOAT3 yawPitchRoll)
{
	DirectX::XMFLOAT3 actualRotation = { -yawPitchRoll.y, yawPitchRoll.x, 0 };
	this->m_gun->setRotation(actualRotation);
	this->m_hands->setRotation(actualRotation);

	DirectX::XMVECTOR new_offset = DirectX::XMLoadFloat3(&m_gunOffset);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(actualRotation.x, actualRotation.y, 0);
	new_offset = DirectX::XMVector3Transform(new_offset, rotationMatrix);

	// reusing
	DirectX::XMStoreFloat3(&yawPitchRoll, new_offset);

	this->m_gun->setPosition(yawPitchRoll);

	// set up the barrel offset
	DirectX::XMVECTOR newBarrelOffset = DirectX::XMLoadFloat3(&m_initialBarrelOffset);
	newBarrelOffset = DirectX::XMVector3Transform(newBarrelOffset, rotationMatrix);
	DirectX::XMStoreFloat3(&m_barrelOffset, newBarrelOffset);
}

