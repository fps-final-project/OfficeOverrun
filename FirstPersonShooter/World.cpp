#include "pch.h"
#include "World.h"
#include "ctime"
#include "AnimatedObjectBuilder.hpp"
#include "ResourceManager.h"

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

			for (auto nextNextRoomIdx : m_rooms[nextRoomIdx].getAdjacentRooms())
			{
				m_visibleRooms.insert(nextNextRoomIdx);
			}
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

std::vector<std::shared_ptr<Hittable>> World::GetHittableEntities() 
{
	std::vector<std::shared_ptr<Hittable>> entities{};
	int size = m_entities.size() + m_animatedEntities.size();
	entities.reserve(size);
	for (const auto& [_, entity] : m_entities)
	{
		entities.push_back((std::shared_ptr<Hittable>)entity);
	}
	for (const auto& [_, entity] : m_animatedEntities)
	{
		entities.push_back((std::shared_ptr<Hittable>)entity);
	}

	return entities;
}

void World::DeleteEntity(const GUID& entityId)
{
	if (m_enemies.find(entityId) != m_enemies.end())
	{
		m_enemies.erase(entityId);
		m_entities.erase(entityId);
		m_animatedEntities.erase(entityId);
	}
}

void World::AddObject(std::shared_ptr<Object>& object)
{
	m_entities[object->id] = object;
}

void World::AddAnimatedObject(std::shared_ptr<AnimatedObject>& object)
{
	m_animatedEntities[object->id] = object;
}

void World::AddEnemy(std::shared_ptr<Enemy>& enemy)
{
	m_animatedEntities[enemy->id] = enemy;
	m_enemies[enemy->id] = enemy;
}

void World::AddHelicopter()
{
	auto& lastRoom = m_rooms[m_rooms.size() - 1];
	auto stairsPos = lastRoom.m_links[0].pos;

	float mid_room_x = lastRoom.getPosition().x + lastRoom.getSize().x / 2;
	float mid_room_z = lastRoom.getPosition().z + lastRoom.getSize().z / 2;

	float v_mid_x = mid_room_x - stairsPos.x;
	float v_mid_z = mid_room_z - stairsPos.z;

	m_helicopterPos = { stairsPos.x + 2 * v_mid_x, lastRoom.getPosition().y, stairsPos.z + 2 * v_mid_z };

	auto heli = AnimatedObjectBuilder()
		.WithNewObject(ResourceManager::Instance().getAnimatedModel("heli"))
		.WithPosition(m_helicopterPos)
		.WithRotation({ 0.f, 0.f, 0.f })
		.WithVelocity({ 0.f, 0.f, 0.f })
		.WithSize({ 2.f, 2.f, 2.f })
		.WithFallbackAnimation("Idle")
		.Build();

	AddAnimatedObject(heli);
}

bool World::IsPlayerNearHelicopter(DirectX::XMFLOAT3 playerPos)
{
	float distanceThreshold = 2.f;

	return playerPos.y > m_helicopterPos.y &&
		(playerPos.x - m_helicopterPos.x) * (playerPos.x - m_helicopterPos.x) +
		(playerPos.z - m_helicopterPos.z) * (playerPos.z - m_helicopterPos.z) < distanceThreshold * distanceThreshold;
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

void World::UpdateEnemies(const Room& room,
	DirectX::XMFLOAT3 playerPos, 
	std::shared_ptr<std::queue<Action>>& actionQueue)
{
	for (const auto& [_, enemy] : m_enemies)
	{
		Action action = enemy->Update(room, m_rooms, playerPos);
		if (action.type != ActionType::NOACTION)
		{
			actionQueue->push(action);
		}
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
