#include "pch.h"
#include "World.h"
#include "ctime"
#include "AnimatedObjectBuilder.hpp"
#include "ResourceManager.h"
#include "Player.h"
#include "EnemyBuilder.hpp"

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

World::World() : gen(std::random_device{}())
{
	auto gun = AnimatedObjectBuilder()
		.WithNewObject(ResourceManager::Instance().getAnimatedModel("ak_gun"))
		.WithPosition({ 2.0f, 0.5f, 3.0f })
		.WithSize({ 0.5f, 0.5f, 0.5f })
		.Build();

	AddGun(std::make_shared<Gun>(Gun(*gun.get(), "ak")));
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

	for (auto& gun : m_guns)
	{
		XMFLOAT3 oldRotation = gun.second->getRotation();
		XMFLOAT3 newRotation = { oldRotation.x, oldRotation.y + dt * XM_PI, oldRotation.z};
		gun.second->setRotation(newRotation);
		
	}

	if (lastDamage < 100.f)
		lastDamage += dt;
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

void World::DeleteEnemy(const GUID& entityId)
{
	if (m_enemies.find(entityId) != m_enemies.end())
	{
		m_enemies.erase(entityId);
		m_entities.erase(entityId);
		m_animatedEntities.erase(entityId);
	}
}

void World::DeleteGun(const GUID& entityId)
{
	if (m_guns.find(entityId) != m_guns.end())
	{
		m_guns.erase(entityId);
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

void World::AddGun(std::shared_ptr<Gun>& gun)
{
	m_guns[gun->id] = gun;
	m_animatedEntities[gun->id] = gun;
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

bool World::IsPlayerNearGun(DirectX::XMFLOAT3 playerPos, std::string& name)
{
	float distanceThreshold = 1.f;
	for (const auto& [_, gun] : m_guns)
	{
		if (playerPos.y > gun->getPosition().y &&
			(playerPos.x - gun->getPosition().x) * (playerPos.x - gun->getPosition().x) +
			(playerPos.z - gun->getPosition().z) * (playerPos.z - gun->getPosition().z) < distanceThreshold * distanceThreshold)
		{
			name = gun->GetGunName();
			DeleteGun(gun->id);
			return true;
		}
	}
	return false;
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

void World::UpdateEnemies(std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos,
	std::shared_ptr<std::queue<Action>>& actionQueue, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	std::vector<GUID> enemiesToDelete;
	auto secondNeighbors = GetSetOfSecondNeighbours(m_currentRoomIndex);

	for (const auto& [_, enemy] : m_enemies)
	{
		Action action = enemy->Update(pathfinder, playerPos);
		if (action.type != ActionType::NOACTION)
		{
			actionQueue->push(action);
		}

		if (!enemy->inCloseProximity() && std::none_of(secondNeighbors.begin(), secondNeighbors.end(), [&](int roomId) {
			return m_rooms[roomId].insideRoom(enemy->getPosition());
			}))
		{
			enemiesToDelete.push_back(enemy->id);
		}
	}

	for (const auto& enemyId : enemiesToDelete)
		DeleteEnemy(enemyId);

	SpawnEnemyNearPlayer(m_enemies.size(), pathfinder, deviceResources);
}

void World::SpawnEnemyNearPlayer(int currentEnemiesNearPlayer,
	std::shared_ptr<Pathfinder> pathfinder, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	const int maxEnemiesNearPlayer = 2;
	const float wallOffset = 0.5f;
	if (currentEnemiesNearPlayer < maxEnemiesNearPlayer)
	{
		auto neighbors = GetSetOfSecondNeighbours(m_currentRoomIndex);
		int random_neighbor = *std::next(neighbors.begin(), std::uniform_int_distribution<int>(0, neighbors.size() - 1)(gen));

		auto random_float = std::uniform_real_distribution<float>();
		
		auto roomPosition = m_rooms[random_neighbor].getPosition();
		auto roomSize = m_rooms[random_neighbor].getSize();

		DirectX::XMFLOAT3 enemyPos = {
			roomPosition.x + wallOffset + random_float(gen) * (roomSize.x - 2 * wallOffset),
			roomPosition.y,
			roomPosition.z + wallOffset + random_float(gen) * (roomSize.z - 2 * wallOffset)
		};


		EnemyBuilder enemyBuilder{};
		auto zombie = enemyBuilder
			.WithNewEnemy(ResourceManager::Instance().getAnimatedModel("zombie_war"))
			.WithSound(ResourceManager::Instance().getAudioFile("zombie"), deviceResources->GetXAudio())
			.WithDamageSound(ResourceManager::Instance().getAudioFile("zombie_dying"), deviceResources->GetXAudio())
			.WithHealth(100)
			.WithDamage(10)
			.WithSpeed(0.05f)
			.WithPosition(enemyPos)
			.WithRotation({ 0.f, 0.f, 0.f })
			.WithVelocity({ 0.f, 0.f, 0.f })
			.WithSize({ 0.8f, 0.8f, 0.8f })
			.WithAttackRadius(0.7f)
			.WithFallbackAnimation("run")
			.WithPath(pathfinder)
			.Build();

		AddEnemy(zombie);
	}

}

std::set<int> World::GetSetOfSecondNeighbours(int roomId)
{
	std::set<int> result;
	for (auto roomIdx : m_rooms[roomId].getAdjacentRooms())
	{
		for (auto nextRoomIdx : m_rooms[roomIdx].getAdjacentRooms())
		{
			result.insert(nextRoomIdx);
		}
	}

	for (auto roomIdx : m_rooms[roomId].getAdjacentRooms())
		result.erase(roomIdx);

	result.erase(roomId);

	return result;
}

void World::PlayEnemySounds(std::shared_ptr<DX::DeviceResources> deviceResources, Player* player) const
{
	for (const auto& [_, enemy] : m_enemies)
	{
		if (!enemy->getSound()->IsPlaying() && rand() % 80 != 0)
			continue;

		enemy
			->getSound()
			->SetEmmiterSettings(
				enemy->getEmitter().get(),
				player->getAudioListener().get(),
				deviceResources->GetX3DInstance(), 
				deviceResources->GetMasteringVoice()
			);

		enemy->getSound()->PlaySound(false);

	}
}

LightingData World::GetLightingData()
{
	LightingData data;
	//data.lightPositions.push_back({ 1.5f, 2.f, 2.f });
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
