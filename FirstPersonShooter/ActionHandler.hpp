#pragma once
#include <queue>
#include <memory>

#include "ActionTypes.hpp"
#include "GunRig.h"
#include "World.h"
#include "Camera.hpp"

class ActionHandler
{
public:
	ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue);
	void HandleActions(GunRig* gunRig, World* world, Camera* camera);
private:
	std::shared_ptr<std::queue<Action>> m_actionQueue;
};

