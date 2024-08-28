#include "pch.h"
#include "SimpleCollisionDetector.hpp"

std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> CollisionDetector::GetCollisions()
{
	std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> collisions{};

	for (int i = 0; i < elements.size(); i++)
	{
		for (int j = i + 1; j < elements.size(); j++)
		{
			if (elements[i].get()->Hit(*(elements[j].get())))
			{
				collisions.emplace_back(elements[i].get(), elements[j].get());
			}
		}
	}

	return collisions;
}