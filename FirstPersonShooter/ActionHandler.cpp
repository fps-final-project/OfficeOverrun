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
	const float defaultGravity = 20.f;
	DirectX::XMFLOAT3 new_acceleration({ 0, 0, 0 });

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
				world->DeleteEntity(m_lastHitEntity);
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

			new_acceleration = { new_acceleration.x + normalizedAt.x, new_acceleration.y, new_acceleration.z + normalizedAt.z };
			break;
		}

		case Action::WALK_BACKWARD:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			new_acceleration = { new_acceleration.x - normalizedAt.x, new_acceleration.y, new_acceleration.z - normalizedAt.z };
			break;
		}

		case Action::WALK_LEFT:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			new_acceleration = { new_acceleration.x + normalizedAt.z, new_acceleration.y, new_acceleration.z - normalizedAt.x };
			break;
		}
		case Action::WALK_RIGHT:
		{
			auto walkDirection = camera->getAt();
			walkDirection.m128_f32[1] = 0.f;
			DirectX::XMFLOAT3 normalizedAt;
			DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

			new_acceleration = { new_acceleration.x - normalizedAt.z, new_acceleration.y, new_acceleration.z + normalizedAt.x };
			break;
		}
		case Action::JUMP:
		{
			player->jump();
			break;
		}
		case Action::WEAPON1:
		{
			player->getGunRig()->ChangeGun("ak");
			break;
		}
		case Action::WEAPON2:
		{
			player->getGunRig()->ChangeGun("FN");
			break;
		}
		case Action::WEAPON3:
		{
			player->getGunRig()->ChangeGun("smg");
			break;
		}
		case Action::WEAPON4:
		{
			player->getGunRig()->ChangeGun("sniper");
			break;
		}

		}
		}

	auto normalized_acceleration = DirectX::XMVector3Normalize({ new_acceleration.x, new_acceleration.y, new_acceleration.z });
	DirectX::XMFLOAT3 acc;
	DirectX::XMStoreFloat3(&acc, normalized_acceleration);
	player->setAcceleration({acc.x, -defaultGravity, acc.z});

}
