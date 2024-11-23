#pragma once
#include <vector>
#include <queue>
#include "Object.hpp"
#include "AnimatedObject.hpp"
#include "Enemy.hpp"
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 
#include "Room.hpp" 
#include "RenderQueue.hpp"
#include "LightingData.hpp"
#include "ActionTypes.hpp"
#include <map>
#include <set>

struct GUIDComparer
{
	bool operator()(const GUID& Left, const GUID& Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};


class __declspec(dllexport) World
{
	std::set<int> m_visibleRooms;
	DirectX::XMFLOAT3 m_helicopterPos;
public:
	void Update(float dt);
	//std::vector<Hittable> GetEntities();
	void DeleteEntity(const GUID& entity);
	void UpdateVisibleRooms();

	std::map<GUID, std::shared_ptr<AnimatedEntity>, GUIDComparer> m_animatedEntities;
	std::map<GUID, std::shared_ptr<Entity>, GUIDComparer> m_entities;
	std::map<GUID, std::shared_ptr<Enemy>, GUIDComparer> m_enemies;
	
	std::vector<Room> m_rooms;
	int m_currentRoomIndex;
	Room& GetCurrentRoom() { return m_rooms[m_currentRoomIndex]; };

	void AddObject(std::shared_ptr<Object>& object);
	void AddAnimatedObject(std::shared_ptr<AnimatedObject>& object);
	void AddEnemy(std::shared_ptr<Enemy>& enemy);

	void AddHelicopter();
	bool IsPlayerNearHelicopter(DirectX::XMFLOAT3 playerPos);

	void UpdateCurrentRoom(DirectX::XMFLOAT3 playerPos);
	void UpdateEnemies(std::shared_ptr<Pathfinder> pathfinder, std::shared_ptr<std::queue<Action>>& actionQueue);

	LightingData GetLightingData();
	RenderQueue CreateRenderQueue();
};
