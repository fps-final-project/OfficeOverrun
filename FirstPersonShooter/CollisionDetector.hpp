#pragma once

#include <vector>
#include <Hittable.hpp>

class CollisionDetector
{

public:
	virtual ~CollisionDetector() = default;

	virtual std::vector<std::pair<Hittable, Hittable>> GetCollisions(std::vector<Hittable> elements) = 0;

protected:
};

