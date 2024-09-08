#pragma once

#include <DirectXCollision.h>

class Hittable
{
public:
	Hittable(DirectX::BoundingBox box);

	bool Hit(const Hittable& other);
	uint32_t id;
	static uint32_t idCounter;
protected:
	DirectX::BoundingBox boundingBox;
};

