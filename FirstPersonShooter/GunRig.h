#pragma once
#include "AnimatedEntity.hpp"
#include <memory> 



class GunRig 
{

public:
	GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset, DirectX::XMFLOAT3 barrelOffset);
	void update(float dt);
	void reload();
	void shoot();
	bool isIdle() { return m_hands->isIdle(); }
	DirectX::XMFLOAT3 getBarrelOffset();
	std::vector<std::shared_ptr<AnimatedEntity>> getEntites();
	void rotate(DirectX::XMFLOAT3 yawPitchRoll);
private:
	DirectX::XMFLOAT3 m_gunOffset, m_initialBarrelOffset, m_barrelOffset;
	std::shared_ptr<AnimatedEntity> m_hands, m_gun;

};