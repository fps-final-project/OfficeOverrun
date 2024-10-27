#pragma once
#include "AnimatedEntity.hpp"
#include <memory> 



class GunRig : public Drawable
{

public:
	GunRig(std::string gunName);
	static DirectX::XMFLOAT3 CalculateBulletOrientation(DirectX::XMFLOAT3 yawPitchRoll);
	DirectX::XMVECTOR CalculateBulletDirection(DirectX::XMVECTOR cameraAt);
	void Update(float dt);
	void Reload();
	void Shoot();
	void ChangeGun(const std::string& name);
	bool IsIdle() { return m_hands->isIdle(); }
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster);
	DirectX::XMFLOAT3 GetBarrelOffset();
	void RotateAndOffset(DirectX::XMFLOAT3 yawPitchRoll, DirectX::XMFLOAT3 playerPos, float dt);
private:
	DirectX::XMFLOAT3 m_rigOffset, m_gunOffset, m_initialBarrelOffset, m_barrelOffset;
	std::shared_ptr<AnimatedEntity> m_hands, m_gun;
	std::string m_name;

};