#include "pch.h"
#include "ActionHandler.hpp"
#include "ResourceManager.h"
#include "Player.h"

ActionHandler::ActionHandler(std::shared_ptr<std::queue<Action>> actionQueue)
{
	m_actionQueue = actionQueue;
}

void ActionHandler::HandleActions(Player* player, World* world, Camera* camera, DX::DeviceResources* deviceResources)
{
	std::unique_ptr<GunRig>& gunRig = player->getGunRig();
	const float defaultGravity = 20.f;
	DirectX::XMFLOAT3 new_acceleration({ 0, 0, 0 });

	// this could be turned into the observer pattern
	while (!m_actionQueue->empty())
	{
		auto action = m_actionQueue->front();
		m_actionQueue->pop();
		switch (action.type)
		{
			case ActionType::ATTACK:
			{
				player->takeDamage(action.args.attack.damage);
				world->lastDamage = 0.0f;
				break;
			}
			case ActionType::SHOOT:
			{
				if (gunRig->IsIdle())
				{
					gunRig->Shoot();
					auto entity = world->m_enemies.find(m_lastHitEntity);
					if (entity != world->m_enemies.end())
					{
						if (entity->second->getDamageSound() != nullptr) {
							entity->second->getDamageSound()->SetEmmiterSettings(
								entity->second->getEmitter().get(),
								player->getAudioListener().get(),
								deviceResources->GetX3DInstance(),
								deviceResources->GetMasteringVoice()
							);
							entity->second->getDamageSound()->PlaySound(true);
						}
						entity->second->takeDamage(gunRig->GetDamage());
						if (entity->second->isDead()) {
							world->DeleteEnemy(m_lastHitEntity);
						}
					}
				}
				break;
			}
			case ActionType::RELOAD:
			{
				if (gunRig->IsIdle())
				{
					gunRig->Reload();
				}
				break;
			}
			case ActionType::WALK_FORWARD:
			{
				auto walkDirection = camera->getAt();
				walkDirection.m128_f32[1] = 0.f;
				DirectX::XMFLOAT3 normalizedAt;
				DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

				new_acceleration = { new_acceleration.x + normalizedAt.x, new_acceleration.y, new_acceleration.z + normalizedAt.z };
				break;
			}

			case ActionType::WALK_BACKWARD:
			{
				auto walkDirection = camera->getAt();
				walkDirection.m128_f32[1] = 0.f;
				DirectX::XMFLOAT3 normalizedAt;
				DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

				new_acceleration = { new_acceleration.x - normalizedAt.x, new_acceleration.y, new_acceleration.z - normalizedAt.z };
				break;
			}

			case ActionType::WALK_LEFT:
			{
				auto walkDirection = camera->getAt();
				walkDirection.m128_f32[1] = 0.f;
				DirectX::XMFLOAT3 normalizedAt;
				DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

				new_acceleration = { new_acceleration.x + normalizedAt.z, new_acceleration.y, new_acceleration.z - normalizedAt.x };
				break;
			}
			case ActionType::WALK_RIGHT:
			{
				auto walkDirection = camera->getAt();
				walkDirection.m128_f32[1] = 0.f;
				DirectX::XMFLOAT3 normalizedAt;
				DirectX::XMStoreFloat3(&normalizedAt, DirectX::XMVector3Normalize(walkDirection));

				new_acceleration = { new_acceleration.x - normalizedAt.z, new_acceleration.y, new_acceleration.z + normalizedAt.x };
				break;
			}
			case ActionType::JUMP:
			{
				player->jump();
				break;
			}
			case ActionType::WEAPON1:
			{
				player->getGunRig()->ChangeGun("ak", deviceResources->GetXAudio());
				break;
			}
			case ActionType::WEAPON2:
			{
				player->getGunRig()->ChangeGun("FN", deviceResources->GetXAudio());
				break;
			}
			case ActionType::WEAPON3:
			{
				player->getGunRig()->ChangeGun("smg", deviceResources->GetXAudio());
				break;
			}
			case ActionType::WEAPON4:
			{
				player->getGunRig()->ChangeGun("sniper", deviceResources->GetXAudio());
				break;
			}
		}
	}

	auto normalized_acceleration = DirectX::XMVector3Normalize({ new_acceleration.x, new_acceleration.y, new_acceleration.z });
	DirectX::XMFLOAT3 acc;
	DirectX::XMStoreFloat3(&acc, normalized_acceleration);
	player->setAcceleration({acc.x, -defaultGravity, acc.z});
}
