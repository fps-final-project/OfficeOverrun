#include "pch.h"
#include "SimpleCollisionDetector.hpp"


std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> SimpleCollisionDetector::GetCollisions(std::vector<std::shared_ptr<Hittable>> elements)
{
	std::vector<std::pair<std::shared_ptr<Hittable>, std::shared_ptr<Hittable>>> collisions{};

	for (int i = 0; i < elements.size(); i++)
	{
		for (int j = i + 1; j < elements.size(); j++)
		{
			if (elements[i]->Hit(*elements[j]))
			{
				collisions.emplace_back(elements[i], elements[j]);
			}
		}
	}

	return collisions;
}