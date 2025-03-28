#include "pch.h"
#include "Player.h"
#include "ResourceManager.h"

Player::Player(IXAudio2* xaudio)
	: m_position({0.f, 0.f, 0.f}), m_velocity({0.f, 0.f, 0.f}),
	m_gunRig(std::make_unique<GunRig>("FN", xaudio)), m_isOnGround(true), m_health(100)
{
	m_listener = std::make_shared<X3DAUDIO_LISTENER>();
	m_listener->OrientTop = { 0.f, 1.f, 0.f };
	m_listener->OrientFront = { 0.f, 1.f, 0.f };
	m_listener->Position = { m_position.x, m_position.y, m_position.z };
	m_listener->Velocity = { m_velocity.x, m_velocity.y, m_velocity.z };
}

void Player::jump()
{
	const float initialJumpVelocity = 6.5f;
	if (m_isOnGround)
	{
		m_velocity.y = initialJumpVelocity;
		m_isOnGround = false;
	}
}

void Player::Update(float dt)
{
	// i cant put it here because i need to first update the player position, then update the gun rig position and offset
	// and then update the gun rig model matricies
	//m_gunRig->Update(dt);


	const float velocityCap = 5.f;
	const float max_slowoff = 0.7f * 60;
	const float min_velocity = 0.1f;

	this->updateVelocity(dt);

	m_velocity.x -= (m_velocity.x / velocityCap) * max_slowoff * dt;
	m_velocity.z -= (m_velocity.z / velocityCap) * max_slowoff * dt;

	if (std::abs(m_velocity.x) < min_velocity)
		m_velocity.x = 0;
	
	if (std::abs(m_velocity.z) < min_velocity)
		m_velocity.z = 0;


	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;
	m_position.z += m_velocity.z * dt;

	m_listener->Position = { m_position.x, m_position.y, m_position.z };
	m_listener->Velocity = { m_velocity.x, m_velocity.y, m_velocity.z };

}

void Player::updateVelocity(float dt)
{
	const float velocityCap = 5.f;
	const float accelerationCoeff = 50.f;
	
	m_velocity.x += m_acceleration.x * dt * accelerationCoeff;
	m_velocity.y += m_acceleration.y * dt;
	m_velocity.z += m_acceleration.z * dt * accelerationCoeff;

	if (m_velocity.x > velocityCap)
		m_velocity.x = velocityCap;

	if (m_velocity.x < -velocityCap)
		m_velocity.x = -velocityCap;
	
	if (m_velocity.z > velocityCap)
		m_velocity.z = velocityCap;

	if (m_velocity.z < -velocityCap)
		m_velocity.z = -velocityCap;
	
}

void Player::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	m_gunRig->Render(renderMaster);
}

void Player::setListenerDirection(DirectX::XMFLOAT3 forward)
{
	m_listener->OrientFront = forward;
}

std::unique_ptr<GunRig>& Player::getGunRig()
{
	return m_gunRig;
}

DirectX::XMFLOAT3 Player::getPostition()
{
	return m_position;
}

void Player::setAcceleration(DirectX::XMFLOAT3 acc)
{
	m_acceleration = acc;
}

void Player::handleRoomCollision(const RoomCollision& collisionData)
{

	// x axis
	if (collisionData.collision[0])
	{
		this->m_velocity.x = 0;
		this->m_position.x = collisionData.correction[0];
	}
	// y axis
	if (collisionData.collision[1])
	{
		this->m_velocity.y = 0;
		this->m_position.y = collisionData.correction[1];
		if (collisionData.isOnGround)
			m_isOnGround = true;
	}
	// z axis
	if (collisionData.collision[2])
	{
		this->m_velocity.z = 0;
		this->m_position.z = collisionData.correction[2];
	}
}

void Player::takeDamage(int damage)
{
	m_health -= damage;
}
