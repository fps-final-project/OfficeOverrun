#include "pch.h"
#include "Hittable.hpp"


uint32_t Hittable::idCounter = 0;

Hittable::Hittable(DirectX::BoundingBox box) : boundingBox{ box }, id{ idCounter++ }
{
}

bool Hittable::Hit(const Hittable& other)
{
	return this->boundingBox.Intersects(other.boundingBox);
}

