#pragma once
#include "AnimatedEntity.hpp"
#include <memory> 



class GunRig : public Drawable
{

public:
	GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset, DirectX::XMFLOAT3 barrelOffset);
	static DirectX::XMFLOAT3 CalculateBulletOrientation(DirectX::XMFLOAT3 yawPitchRoll);
	DirectX::XMVECTOR CalculateBulletDirection(DirectX::XMVECTOR cameraAt);
	void Update(float dt);
	void Reload();
	void Shoot();
	bool IsIdle() { return m_hands->isIdle(); }
	virtual void Render(std::shared_ptr<RenderMaster> renderMaster);
	DirectX::XMFLOAT3 GetBarrelOffset();
	std::vector<std::shared_ptr<AnimatedEntity>> GetEntites();
	void Rotate(DirectX::XMFLOAT3 yawPitchRoll);
private:
	DirectX::XMFLOAT3 m_gunOffset, m_initialBarrelOffset, m_barrelOffset;
	std::shared_ptr<AnimatedEntity> m_hands, m_gun;

};