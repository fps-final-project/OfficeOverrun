#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 

class World
{
public:
	void update(float dt);
	std::vector<AnimatedEntity> m_entities;
};