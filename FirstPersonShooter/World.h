#pragma once
#include <vector>
#include <queue>
#include "Object.hpp"
#include "AnimatedObject.hpp"
#include "Enemy.hpp"
#include "Gun.hpp"
#include "AnimatedEntity.hpp" 
#include "Entity.hpp" 
#include "Room.hpp" 
#include "RenderQueue.hpp"
#include "ActionTypes.hpp"
#include <map>
#include <set>
#include <random>

class Player;

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
	std::mt19937 gen;

	void SpawnEnemyInRoom(int room_idx, std::shared_ptr<Pathfinder> pathfinder, std::shared_ptr<DX::DeviceResources> deviceResources);
public:
	World();

	void Update(float dt);
	std::vector<std::shared_ptr<Hittable>> GetHittableEntities();
	void UpdateVisibleRooms();

	std::map<GUID, std::shared_ptr<AnimatedEntity>, GUIDComparer> m_animatedEntities;
	std::map<GUID, std::shared_ptr<Entity>, GUIDComparer> m_entities;
	std::map<GUID, std::shared_ptr<Enemy>, GUIDComparer> m_enemies;
	std::map<GUID, std::shared_ptr<Gun>, GUIDComparer> m_guns;
	
	std::vector<Room> m_rooms;
	int m_currentRoomIndex;
	Room& GetCurrentRoom() { return m_rooms[m_currentRoomIndex]; };

	void AddObject(std::shared_ptr<Object>& object);
	void AddAnimatedObject(std::shared_ptr<AnimatedObject>& object);
	void AddEnemy(std::shared_ptr<Enemy>& enemy);
	void AddGun(std::shared_ptr<Gun>& gun);
	void DeleteEnemy(const GUID& entity);
	void DeleteGun(const GUID& entity);

	void AddHelicopter();
	bool IsPlayerNearHelicopter(DirectX::XMFLOAT3 playerPos);
	bool IsPlayerNearGun(DirectX::XMFLOAT3 playerPos, std::string& name);

	void UpdateCurrentRoom(DirectX::XMFLOAT3 playerPos);
	void PlayEnemySounds(std::shared_ptr<DX::DeviceResources> deviceResources, Player* player) const;
	void UpdateEnemies(float dt, std::shared_ptr<Pathfinder> pathfinder, DirectX::XMFLOAT3 playerPos,
		std::shared_ptr<std::queue<Action>>& actionQueue, std::shared_ptr<DX::DeviceResources> deviceResources);

	void SpawnBaseEnemies(std::shared_ptr<Pathfinder> pathfinder, std::shared_ptr<DX::DeviceResources> deviceResources);
	void SpawnEnemyNearPlayer(int currentEnemiesNearPlayer, std::shared_ptr<Pathfinder> pathfinder, std::shared_ptr<DX::DeviceResources> deviceResources);
	std::set<int> GetSetOfSecondNeighbours(int roomId);

	void SpawnBaseGuns();

	RenderQueue CreateRenderQueue();

	float lastDamage = 4.f;
};
