#include "pch.h"
#include "Player.h"
#include "ResourceManager.h"

Player::Player()
	: m_position({0.f, 0.f, 0.f}), m_velocity({0.f, 0.f, 0.f}),
	m_gunRig(std::make_unique<GunRig>(
		ResourceManager::Instance.getAnimatedModel("myarms"),
		ResourceManager::Instance.getAnimatedModel("mygun"),
		XMFLOAT3(0.f, 0.f, 0.4572f),
		XMFLOAT3(-0.118846, -0.106299, 0.55291)))
{
}

void Player::Update(float dt)
{
	// so i cant put it here because i need to first update the player position, then update the gun rig position and offset
	// and then update the gun rig model matricies
	//m_gunRig->Update(dt);

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;
	m_position.z += m_velocity.z * dt;

	float velocityCap = 5.f;
	float max_slowoff = 0.3f;

	m_velocity.x -= (m_velocity.x / velocityCap) * max_slowoff;
	m_velocity.y -= (m_velocity.y / velocityCap) * max_slowoff;
	m_velocity.z -= (m_velocity.z / velocityCap) * max_slowoff;


}

void Player::updateVelocity(DirectX::XMFLOAT3 deltaV)
{
	float velocityCap = 5.f;
	
	m_velocity.x += deltaV.x;
	m_velocity.y += deltaV.y;
	m_velocity.z += deltaV.z;

	if (m_velocity.x > velocityCap)
		m_velocity.x = velocityCap;

	if (m_velocity.x < -velocityCap)
		m_velocity.x = -velocityCap;

	if (m_velocity.y > velocityCap)
		m_velocity.y = velocityCap;

	if (m_velocity.y < -velocityCap)
		m_velocity.y = -velocityCap;

	if (m_velocity.z > velocityCap)
		m_velocity.z = velocityCap;

	if (m_velocity.z < -velocityCap)
		m_velocity.z = -velocityCap;
	
}

void Player::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	m_gunRig->Render(renderMaster);
}

std::unique_ptr<GunRig>& Player::getGunRig()
{
	return m_gunRig;
}

DirectX::XMFLOAT3 Player::getPostition()
{
	return m_position;
}
