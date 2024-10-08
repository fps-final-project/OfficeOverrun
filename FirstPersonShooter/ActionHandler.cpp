#include "pch.h"
#include "ActionHandler.hpp"
#include "ResourceManager.h"
#include "Player.h"

ActionHandler::ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue)
{
	m_actionQueue = actionQueue;
}

void ActionHandler::HandleActions(Player* player, World* world, Camera* camera)
{
	std::unique_ptr<GunRig>& gunRig = player->getGunRig();

	// this could be turned into the observer pattern
	while (!m_actionQueue->empty())
	{
		auto action = m_actionQueue->front();
		m_actionQueue->pop();
		switch (action)
		{
		case Action::SHOOT:
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
			break;
		}
		case Action::RELOAD:
		{
			if (gunRig->IsIdle())
			{
				gunRig->Reload();
			}
			break;
		}
		case Action::WALK_FORWARD:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			player->updateVelocity({ normalizedAt.x, 0, normalizedAt.z });
			break;
		}

		case Action::WALK_BACKWARD:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			player->updateVelocity({ -normalizedAt.x, 0, -normalizedAt.z });
			break;
		}

		case Action::WALK_LEFT:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			player->updateVelocity({ normalizedAt.z, 0, -normalizedAt.x });
			break;
		}
		case Action::WALK_RIGHT:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			player->updateVelocity({ -normalizedAt.z, 0, normalizedAt.x });
			break;
		}

		}
		}
}
