#pragma once
#include "AnimatedEntity.hpp"
#include <memory> 



class GunRig 
{

public:
	GunRig(std::shared_ptr<AnimatedAssimpModel> hands, std::shared_ptr<AnimatedAssimpModel> gun, DirectX::XMFLOAT3 gunOffset);
	void update(float dt);
	void reload();
	void shoot();
	std::vector<std::shared_ptr<AnimatedEntity>> getEntites();
private:
	std::shared_ptr<AnimatedEntity> m_hands, m_gun;

};