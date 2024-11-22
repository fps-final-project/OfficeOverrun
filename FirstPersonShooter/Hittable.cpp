#include "pch.h"
#include "Hittable.hpp"



Hittable::Hittable(DirectX::BoundingBox box, DirectX::XMFLOAT3 position) : boundingBox{ box }, position{ position }
{
}	

bool Hittable::Hit(const Hittable& other)
{
	DirectX::BoundingBox otherBox = other.boundingBox;
	DirectX::BoundingBox thisBox = this->boundingBox;
	

	otherBox.Center = DirectX::XMFLOAT3(
		otherBox.Center.x + other.position.x,
		otherBox.Center.y + other.position.y,
		otherBox.Center.z + other.position.z
	);

	thisBox.Center = DirectX::XMFLOAT3(
		thisBox.Center.x + this->position.x,
		thisBox.Center.y + this->position.y,
		thisBox.Center.z + this->position.z
	);

	return thisBox.Intersects(otherBox);
}

