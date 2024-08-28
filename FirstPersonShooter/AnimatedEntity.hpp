#pragma once
#include "Hittable.hpp"
#include "Animable.hpp"

using namespace DirectX;

class AnimatedEntity : public Hittable, public Animable
{
public:
	AnimatedEntity(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 velocity);
	void Update(float dt);

protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
};