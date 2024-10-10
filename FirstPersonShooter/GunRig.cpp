#include "pch.h"
#include "GunRig.h"
#include <cmath>

GunRig::GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset, DirectX::XMFLOAT3 barrelOffset)
	: m_gunOffset(gunOffset), m_initialBarrelOffset(barrelOffset), m_barrelOffset(barrelOffset)
{
	// idea - they can be shared with world.entites
	m_hands = std::make_shared<AnimatedEntity>(hands, XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, DirectX::XM_PI, 0.f));
	m_gun = std::make_shared<AnimatedEntity>(gun, gunOffset, XMFLOAT3(0.f, DirectX::XM_PI, 0.f));

	m_hands->setFallbackAnimation("FP_idle_pose");
	m_gun->setFallbackAnimation("GUN_idle_pose");
}

DirectX::XMFLOAT3 GunRig::CalculateBulletOrientation(DirectX::XMFLOAT3 yawPitchRoll)
{
	return { 0.f, yawPitchRoll.x - DirectX::XM_PIDIV2, yawPitchRoll.y };
}

DirectX::XMVECTOR GunRig::CalculateBulletDirection(DirectX::XMVECTOR cameraAt)
{
	// first scale the vector to (almost) inf, then subtract barrel position to get the direction from barrel, then normalize
	auto result = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMVectorScale(cameraAt, 1e6),
		DirectX::XMLoadFloat3(&m_barrelOffset)));

	return result;
}

void GunRig::Update(float dt)
{
	this->m_hands->Update(dt);
	this->m_gun->Update(dt);
}

void GunRig::Reload()
{
	this->m_hands->setAnimation("FP_reload");
	this->m_gun->setAnimation("GUN_reload");
}

void GunRig::Shoot()
{
	const float speedup = 1.6f;
	this->m_hands->setAnimation("FP_fire", speedup);
	this->m_gun->setAnimation("GUN_fire", speedup);
}

void GunRig::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	this->m_gun->Render(renderMaster);
	this->m_hands->Render(renderMaster);
}

DirectX::XMFLOAT3 GunRig::GetBarrelOffset()
{
	return m_barrelOffset;
}

void GunRig::RotateAndOffset(DirectX::XMFLOAT3 yawPitchRoll, DirectX::XMFLOAT3 playerPos, float dt)
{
	DirectX::XMFLOAT3 actualRotation = { -yawPitchRoll.y, yawPitchRoll.x + XM_PI, 0 };
	this->m_gun->setRotation(actualRotation);
	this->m_hands->setRotation(actualRotation);

	DirectX::XMVECTOR new_offset = DirectX::XMLoadFloat3(&m_gunOffset);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(actualRotation.x, actualRotation.y, 0);
	new_offset = DirectX::XMVector3Transform(new_offset, rotationMatrix);
	new_offset = DirectX::XMVectorAdd(new_offset, { playerPos.x, playerPos.y, playerPos.z, 0.f });


	DirectX::XMFLOAT3 new_offset_f3;
	DirectX::XMStoreFloat3(&new_offset_f3, new_offset);

	this->m_gun->setPosition(new_offset_f3);
	this->m_hands->setPosition(playerPos);

	// set up the barrel offset
	DirectX::XMVECTOR newBarrelOffset = DirectX::XMLoadFloat3(&m_initialBarrelOffset);
	newBarrelOffset = DirectX::XMVector3Transform(newBarrelOffset, rotationMatrix);
	DirectX::XMStoreFloat3(&m_barrelOffset, DirectX::XMVectorAdd(newBarrelOffset, { playerPos.x, playerPos.y, playerPos.z, 0.f }));

	this->Update(dt);
}
