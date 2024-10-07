#pragma once
#include <queue>
#include <memory>

#include "ActionTypes.hpp"
#include "World.h"
#include "Camera.hpp"

class Player;

class ActionHandler
{
public:
	ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue);
	void HandleActions(Player* player, World* world, Camera* camera);
private:
	std::shared_ptr<std::queue<Action>> m_actionQueue;
};

