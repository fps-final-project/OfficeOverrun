#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 
#include "Room.hpp" 
#include "RenderQueue.hpp"


class World
{
public:
	void Update(float dt);
	std::vector<Hittable> GetEntities();
	void DeleteEntity(Hittable& entity);

	std::vector<AnimatedEntity> m_animatedEntities;
	std::vector<Entity> m_entities;
	std::vector<Room> m_rooms;
	std::vector<std::pair<Entity, float>> m_timedEntities;

	RenderQueue createRenderQueue();
};