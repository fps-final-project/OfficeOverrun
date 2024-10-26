#include "pch.h"
#include "GunRig.h"
#include <cmath>

GunRig::GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun,
	DirectX::XMFLOAT3 gunOffset, DirectX::XMFLOAT3 barrelOffset, DirectX::XMFLOAT3 rigOffset)
	: m_gunOffset(gunOffset), m_initialBarrelOffset(barrelOffset), m_barrelOffset(barrelOffset), m_rigOffset(rigOffset)
{
	// idea - they can be shared with world.entites
	m_hands = std::make_shared<AnimatedEntity>(hands, XMFLOAT3(0.f, -1.369078, -0.125759), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(0.f, DirectX::XM_PI, 0.f));
	m_gun = std::make_shared<AnimatedEntity>(gun, gunOffset, XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(0.f, DirectX::XM_PI, 0.f));

	m_hands->setFallbackAnimation("AK/idle");
	m_gun->setFallbackAnimation("AK/idle");
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
	this->m_hands->setAnimation("AK/reload2");
	this->m_gun->setAnimation("AK/reload2");
}

void GunRig::Shoot()
{
	const float speedup = 1.6f;
	this->m_hands->setAnimation("AK/shoot", speedup);
	this->m_gun->setAnimation("AK/shoot", speedup);
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
	DirectX::XMVECTOR rig_offset = DirectX::XMLoadFloat3(&m_rigOffset);

	DirectX::XMFLOAT3 actualRotation = { -yawPitchRoll.y, yawPitchRoll.x + XM_PI, 0 };
	this->m_gun->setRotation(actualRotation);
	this->m_hands->setRotation(actualRotation);

	DirectX::XMVECTOR new_offset = DirectX::XMLoadFloat3(&m_gunOffset);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(actualRotation.x, actualRotation.y, 0);
	new_offset = DirectX::XMVector3Transform(new_offset, rotationMatrix);
	rig_offset = DirectX::XMVector3Transform(rig_offset, rotationMatrix);

	auto rigPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&playerPos), rig_offset);
	new_offset = DirectX::XMVectorAdd(new_offset, rigPos);


	DirectX::XMFLOAT3 new_offset_f3;
	DirectX::XMStoreFloat3(&new_offset_f3, new_offset);

	this->m_gun->setPosition(new_offset_f3);

	DirectX::XMFLOAT3 rig_offset_f3;
	DirectX::XMStoreFloat3(&rig_offset_f3, rigPos);


	this->m_hands->setPosition(rig_offset_f3);

	// set up the barrel offset
	DirectX::XMVECTOR newBarrelOffset = DirectX::XMLoadFloat3(&m_initialBarrelOffset);
	newBarrelOffset = DirectX::XMVector3Transform(newBarrelOffset, rotationMatrix);
	DirectX::XMStoreFloat3(&m_barrelOffset, DirectX::XMVectorAdd(newBarrelOffset, { playerPos.x, playerPos.y, playerPos.z, 0.f }));

	this->Update(dt);
}
