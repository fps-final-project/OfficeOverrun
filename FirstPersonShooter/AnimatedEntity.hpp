#pragma once
#include "Hittable.hpp"
#include "Animable.hpp"

using namespace DirectX;

class AnimatedEntity : public Hittable, public Animable
{
public:
	AnimatedEntity(std::shared_ptr<AnimatedAssimpModel> model, 
		XMFLOAT3 position = DirectX::XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3 rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f), 
		XMFLOAT3 velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	void Update(float dt);

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
};