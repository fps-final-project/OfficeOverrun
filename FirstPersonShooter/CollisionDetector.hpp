#pragma once

#include <vector>
#include "Hittable.hpp"

class  CollisionDetector
{

public:
	virtual ~CollisionDetector() = default;

	virtual std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> GetCollisions(std::vector<std::shared_ptr<Hittable>> elements) = 0;

protected:
};

