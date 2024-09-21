#include "pch.h"
#include "ActionHandler.hpp"
#include "ResourceManager.h"

ActionHandler::ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue)
{
	m_actionQueue = actionQueue;
}

void ActionHandler::HandleActions(GunRig* gunRig, World* world, Camera* camera)
{
	while (!m_actionQueue->empty())
	{
		auto action = m_actionQueue->front();
		m_actionQueue->pop();
		if (action == Action::SHOOT)
		{
			if (gunRig->IsIdle())
			{
				gunRig->Shoot();
				auto vector_at = gunRig->CalculateBulletDirection(camera->getAt());
				XMFLOAT3 v;
				DirectX::XMStoreFloat3(&v, DirectX::XMVectorScale(vector_at, 10.f));

				auto barrelOffset = gunRig->GetBarrelOffset();
				world->m_timedEntities.push_back(std::make_pair(
					Entity(ResourceManager::Instance.getModel("bullet"), barrelOffset,
						{ 1.f, 1.f, 1.f }, GunRig::CalculateBulletOrientation(camera->getYawPitchRoll()), v),
					3.f
				));
			}
		}
		if (action == Action::RELOAD)
		{
			if (gunRig->IsIdle())
			{
				gunRig->Reload();
			}
		}
	}
}
