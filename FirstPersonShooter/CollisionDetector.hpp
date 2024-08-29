#pragma once

#include <vector>
#include <Hittable.hpp>

class CollisionDetector
{

public:
	virtual std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> GetCollisions() = 0;

private:
	std::vector<std::shared_ptr<Hittable>> elements;
};

