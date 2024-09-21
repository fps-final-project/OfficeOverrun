#include "pch.h"
#include "SimpleCollisionDetector.hpp"


std::vector<std::pair<Hittable, Hittable>> SimpleCollisionDetector::GetCollisions(std::vector<Hittable> elements)
{
	std::vector<std::pair<Hittable, Hittable>> collisions{};

	for (int i = 0; i < elements.size(); i++)
	{
		for (int j = i + 1; j < elements.size(); j++)
		{
			if (elements[i].Hit(elements[j]))
			{
				collisions.emplace_back(elements[i], elements[j]);
			}
		}
	}

	return collisions;
}