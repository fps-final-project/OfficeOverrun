#pragma once
#include "CollisionDetector.hpp"

class SimpleCollisionDetector : public CollisionDetector
{
public:
	std::vector<std::pair<Hittable, Hittable>> GetCollisions(std::vector<Hittable> elements) override;
};

