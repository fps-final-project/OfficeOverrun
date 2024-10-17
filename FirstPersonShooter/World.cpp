#include "pch.h"
#include "World.h"
#include "ctime"

void World::Update(float dt)
{
	for (auto& entity : m_entities)
	{
		entity.Update(dt);
	}


	if (m_animatedEntities.size() && m_animatedEntities[0].isIdle())
	{
		int idx = (time(NULL) % 2) + 1;
		m_animatedEntities[0].setAnimation("attack" + std::to_string(idx), 1.5f);
	}

	for (auto& entity : m_animatedEntities)
	{
		entity.Update(dt);
	}
}

std::vector<Hittable> World::GetEntities()
{
	std::vector<Hittable> entities{};
	int size = m_entities.size() + m_animatedEntities.size();
	entities.reserve(size);
	entities.insert(entities.end(), m_entities.begin(), m_entities.end());
	entities.insert(entities.end(), m_animatedEntities.begin(), m_animatedEntities.end());

	return entities;
}

void World::DeleteEntity(const GUID& entityId)
{
	m_entities.erase(
		std::remove_if(
			m_entities.begin(),
			m_entities.end(),
			[&entityId](Drawable& e) { return e.id == entityId; }),
		m_entities.end()
	);

	m_animatedEntities.erase(
		std::remove_if(
			m_animatedEntities.begin(),
			m_animatedEntities.end(),
			[&entityId](Drawable& e) { return e.id == entityId; }
		),
		m_animatedEntities.end()
	);
}

void World::updateCurrentRoom(DirectX::XMFLOAT3 playerPos)
{
	for (auto neighbour : m_rooms[m_currentRoomIndex].getAdjacentRooms())
	{
		if (m_rooms[neighbour].insideRoom(playerPos))
		{
			m_currentRoomIndex = neighbour;
			break;
		}
	}
}

LightingData World::getLightingData()
{
	LightingData data;
	data.lightPositions.push_back({ 1.5f, 2.f, 2.f });
	return data;
}

RenderQueue World::createRenderQueue()
{
	RenderQueue queue;
	for (const auto& entity : m_entities)
	{
		queue.push(RenderData(RendererType::MODEL, (Drawable*)&entity));
	}

	for (const auto& entity : m_animatedEntities)
	{
		queue.push(RenderData(RendererType::ANIMATED, (Drawable*)&entity));
	}

	queue.push(RenderData(RendererType::MODEL, (Drawable*)&m_rooms[m_currentRoomIndex]));
	for (const auto& entity : m_rooms[m_currentRoomIndex].getAdjacentRooms())
	{
		queue.push(RenderData(RendererType::MODEL, (Drawable*)&m_rooms[entity]));
	}


	return queue;
}
