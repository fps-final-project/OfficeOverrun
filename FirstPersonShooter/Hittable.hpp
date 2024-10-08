#pragma once

#include <DirectXCollision.h>

class Hittable
{
public:
	Hittable(DirectX::BoundingBox box);
	virtual ~Hittable() {}

	bool Hit(const Hittable& other);
protected:
	DirectX::BoundingBox boundingBox;
};

