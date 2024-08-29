#pragma once

#include <DirectXCollision.h>

class Hittable
{
public:
	Hittable(DirectX::BoundingBox box);

	bool Hit(const Hittable& other);

protected:
	DirectX::BoundingBox boundingBox;
};

