#pragma once
#include <vector>
#include <queue>
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 
#include "Room.hpp" 
#include "RenderQueue.hpp"
#include "LightingData.hpp"


class World
{
public:
	void Update(float dt);
	std::vector<Hittable> GetEntities();
	void DeleteEntity(const GUID& entity);

	std::vector<AnimatedEntity> m_animatedEntities;
	std::vector<Entity> m_entities;
	std::vector<Room> m_rooms;
	int m_currentRoomIndex;

	Room& getCurrentRoom() { return m_rooms[m_currentRoomIndex]; };

	LightingData getLightingData();

	RenderQueue createRenderQueue();
};