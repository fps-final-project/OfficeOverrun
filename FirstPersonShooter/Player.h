#pragma once
#include <DirectXMath.h> 
#include "GunRig.h"
#include "Camera.hpp"
#include "RoomCollision.hpp"
#include <x3daudio.h>


class __declspec(dllexport) Player : public Drawable
{
public:
	Player(IXAudio2* xaudio);
	void jump();
	void Update(float dt);
	void updateVelocity(float dt);
	void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	std::unique_ptr<GunRig>& getGunRig();
	DirectX::XMFLOAT3 getPostition();

	void setAcceleration(DirectX::XMFLOAT3 acc);
	void handleRoomCollision(const RoomCollision& collisionData);
	void takeDamage(int damage);
	inline int getHealth() { return m_health; }
	inline std::shared_ptr<X3DAUDIO_LISTENER> getAudioListener() { return m_listener; }
private:
	DirectX::XMFLOAT3 m_position, m_velocity, m_acceleration;
	std::unique_ptr<GunRig> m_gunRig;
	bool m_isOnGround;
	int m_health;
	std::shared_ptr<X3DAUDIO_LISTENER> m_listener;
};