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
	void takeDamage(int damage);
	void updateVelocity(float dt);
	void setAcceleration(DirectX::XMFLOAT3 acc);
	void handleRoomCollision(const RoomCollision& collisionData);
	void Render(std::shared_ptr<RenderMaster> renderMaster) override;
	std::unique_ptr<GunRig>& getGunRig();
	DirectX::XMFLOAT3 getPostition();
	std::pair<int, int> getAmmoCapacity() { return std::make_pair(m_gunRig->GetClipAmmo(), m_gunRig->GetTotalAmmo()); }


	inline int getHealth() { return m_health; }
	inline bool isDead() { return m_health <= 0; }
	inline std::shared_ptr<X3DAUDIO_LISTENER> getAudioListener() { return m_listener; }
private:
	DirectX::XMFLOAT3 m_position, m_velocity, m_acceleration;
	std::unique_ptr<GunRig> m_gunRig;
	std::shared_ptr<X3DAUDIO_LISTENER> m_listener;
	bool m_isOnGround;
	int m_health;
};