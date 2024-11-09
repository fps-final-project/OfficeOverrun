#include "pch.h"
#include "World.h"
#include "ctime"

void World::UpdateVisibleRooms()
{
	m_visibleRooms.clear();
	m_visibleRooms.insert(m_currentRoomIndex);

	for (auto roomIdx : m_rooms[m_currentRoomIndex].getAdjacentRooms())
	{
		m_visibleRooms.insert(roomIdx);
		for (auto nextRoomIdx : m_rooms[roomIdx].getAdjacentRooms())
		{
			m_visibleRooms.insert(nextRoomIdx);

			/*for (auto nextNextRoomIdx : m_rooms[nextRoomIdx].getAdjacentRooms())
			{
				m_visibleRooms.insert(nextNextRoomIdx);
			}*/
		}
	}
}

void World::Update(float dt)
{
	// maybe parallelism?
	for (auto& entity : m_entities)
	{
		entity.second->Update(dt);
	}

	for (auto& entity : m_animatedEntities)
	{
		entity.second->Update(dt);
	}
	/*if (m_animatedEntities.size() && m_animatedEntities[0].isIdle())
	{
		int idx = (time(NULL) % 2) + 1;
		m_animatedEntities[0].setAnimation("attack" + std::to_string(idx), 1.5f);
	}*/
}

/*std::vector<Hittable> World::GetEntities()
{
	std::vector<Hittable> entities{};
	int size = m_entities.size() + m_animatedEntities.size();
	entities.reserve(size);
	entities.insert(entities.end(), m_entities.begin(), m_entities.end());
	entities.insert(entities.end(), m_animatedEntities.begin(), m_animatedEntities.end());

	return entities;
}*/

void World::DeleteEntity(const GUID& entityId)
{
	m_entities.erase(entityId);
	m_animatedEntities.erase(entityId);
	m_entities.erase(entityId);
}

void World::AddObject(std::shared_ptr<Object>& object)
{
	m_entities[object->id] = object;
}

void World::AddEnemy(std::shared_ptr<Enemy>& enemy)
{
	m_animatedEntities[enemy->id] = enemy;
	m_enemies[enemy->id] = enemy;
}

void World::UpdateCurrentRoom(DirectX::XMFLOAT3 playerPos)
{
	for (auto neighbour : m_rooms[m_currentRoomIndex].getAdjacentRooms())
	{
		if (m_rooms[neighbour].insideRoom(playerPos))
		{
			m_currentRoomIndex = neighbour;
			this->UpdateVisibleRooms();
			break;
		}
	}
}

void World::UpdateEnemies(DirectX::XMFLOAT3 playerPos)
{
	for (const auto& enemy : m_enemies)
	{
		enemy.second->Move(playerPos);
	}
}

LightingData World::GetLightingData()
{
	LightingData data;
	data.lightPositions.push_back({ 1.5f, 2.f, 2.f });
	return data;
}

RenderQueue World::CreateRenderQueue()
{
	RenderQueue queue;
	for (const auto& entity : m_entities)
	{
		queue.Push(RenderData(RendererType::MODEL, (Drawable*)(entity.second.get())));
	}

	for (const auto& entity : m_animatedEntities)
	{
		queue.Push(RenderData(RendererType::ANIMATED, (Drawable*)(entity.second.get())));
	}

	for (int room : m_visibleRooms)
	{
		queue.Push(RenderData(RendererType::MODEL, (Drawable*)&m_rooms[room]));
	}

	return queue;
}
