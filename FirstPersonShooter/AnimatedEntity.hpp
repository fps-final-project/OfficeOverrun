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
	virtual ~AnimatedEntity() {}
	void Update(float dt);

	void setPosition(DirectX::XMFLOAT3 pos);
	void setRotation(DirectX::XMFLOAT3 rot);

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
};