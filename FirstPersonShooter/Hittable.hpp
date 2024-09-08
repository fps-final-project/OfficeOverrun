#pragma once

#include <DirectXCollision.h>

class Hittable
{
public:
	Hittable(DirectX::BoundingBox box);

	bool Hit(const Hittable& other);
	uint32_t id;
protected:
	DirectX::BoundingBox boundingBox;
};

