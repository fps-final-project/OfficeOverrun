#pragma once

#include "Drawable.hpp"
#include "Hittable.hpp"

using namespace DirectX;

class Entity : public Drawable, public Hittable
{
public:
	virtual void Update(float dt) = 0;
	virtual void Animate() = 0;
protected:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 velocity;
};

