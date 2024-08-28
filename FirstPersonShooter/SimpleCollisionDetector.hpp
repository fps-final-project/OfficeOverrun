#pragma once
#include "CollisionDetector.hpp"

class SimpleCollisionDetector : public CollisionDetector
{
public:

	std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> GetCollisions() override;
};

