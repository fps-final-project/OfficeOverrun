#include "pch.h"
#include "Hittable.hpp"
#include <DirectXCollision.h>


Hittable::Hittable(DirectX::BoundingBox box) : boundingBox{ box }
{
}

bool Hittable::Hit(const Hittable& other)
{
	return this->boundingBox.Intersects(other.boundingBox);
}
