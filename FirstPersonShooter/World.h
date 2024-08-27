#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 

class World
{
public:
	void update(float dt, std::queue<AnimatedModelDrawRequest>& m_drawQueue);
	std::vector<AnimatedEntity> m_entities;
};