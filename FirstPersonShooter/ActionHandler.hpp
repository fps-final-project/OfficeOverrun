#pragma once
#include <queue>
#include <memory>

#include "ActionTypes.hpp"
#include "World.h"
#include "Camera.hpp"

class Player;

class __declspec(dllexport) ActionHandler
{
public:
	ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue);
	void HandleActions(Player* player, World* world, Camera* camera);
	void SetLastHitEntity(GUID entity) { m_lastHitEntity = entity; }
private:
	std::shared_ptr<std::queue<Action>> m_actionQueue;
	GUID m_lastHitEntity;
};

