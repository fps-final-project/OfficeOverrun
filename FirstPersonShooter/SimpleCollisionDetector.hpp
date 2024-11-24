#pragma once
#include "CollisionDetector.hpp"


class __declspec(dllexport) SimpleCollisionDetector : public CollisionDetector
{
public:
	std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> GetCollisions(std::vector<std::shared_ptr<Hittable>> elements) override;
};

