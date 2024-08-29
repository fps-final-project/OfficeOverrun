#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 

class World
{
public:
	void update(float dt);
	std::vector<AnimatedEntity> m_animatedEntities;
	std::vector<Entity> m_entities;
};