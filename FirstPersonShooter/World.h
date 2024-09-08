#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 

class World
{
public:
	void Update(float dt);
	std::vector<Hittable> GetEntities();
	void DeleteEntity(Hittable& entity);

	std::vector<AnimatedEntity> m_animatedEntities;
	std::vector<Entity> m_entities;
	std::vector<std::pair<Entity, float>> m_timedEntities;
};