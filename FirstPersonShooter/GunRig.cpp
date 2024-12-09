#include "pch.h"
#include "GunRig.h"
#include <cmath>
#include "ResourceManager.h"

GunRig::GunRig(std::string gunName, IXAudio2* xaudio) : 
	m_gunSound(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile(gunName), xaudio)), 
	m_reloadSound(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile("reload"), xaudio)),
	m_emptyClip(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile("empty-clip"), xaudio))

{
	m_hands = std::make_shared<AnimatedEntity>(ResourceManager::Instance().getAnimatedModel(gunName));
	m_gun = std::make_shared<AnimatedEntity>(ResourceManager::Instance().getAnimatedModel(gunName + "_gun"));
	 
	this->ChangeGun(gunName, xaudio);
}

DirectX::XMFLOAT3 GunRig::CalculateBulletOrientation(DirectX::XMFLOAT3 yawPitchRoll)
{
	return { 0.f, yawPitchRoll.x - DirectX::XM_PIDIV2, yawPitchRoll.y };
}

DirectX::XMVECTOR GunRig::CalculateBulletDirection(DirectX::XMVECTOR cameraAt)
{
	// first scale the vector to (almost) inf, then subtract barrel position to get the direction from barrel, then normalize
	auto result = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMVectorScale(cameraAt, 1e6),
		DirectX::XMLoadFloat3(&m_barrelOffset)));

	return result;
}

void GunRig::Update(float dt)
{
	this->m_hands->Update(dt);
	this->m_gun->Update(dt);
}

void GunRig::Reload()
{
	if (m_ammo[m_name].second - m_ammo[m_name].first == 0)
	{
		return;
	}
	this->m_hands->setAnimation("reload1");
	this->m_gun->setAnimation("reload1");
	m_reloadSound->PlaySound(false);
	int ammoInClip = m_ammo[m_name].first;
	int ammoInTotal = m_ammo[m_name].second;
	int ammoToReload = m_clipSize - ammoInClip;
	if (ammoInTotal >= ammoToReload)
	{
		m_ammo[m_name].first = m_clipSize;
		m_ammo[m_name].second -= ammoToReload;
	}
	else
	{
		m_ammo[m_name].first += ammoInTotal;
		m_ammo[m_name].second = 0;
	}
}

bool GunRig::Shoot()
{
	if (m_ammo[m_name].first <= 0)
	{
		m_emptyClip->PlaySound(false);
		return false;
	}
	m_ammo[m_name].first--;
	m_ammo[m_name].second--;
	this->m_hands->setAnimation("shoot", m_shootingAnimationSpeedup, false);
	this->m_gun->setAnimation("shoot", m_shootingAnimationSpeedup, false);
	m_gunSound->PlaySound(true);
	return true;
}

void GunRig::ChangeGun(const std::string& name, IXAudio2* xaudio)
{
	std::shared_ptr<GunRigMetadata> data = ResourceManager::Instance().getGunRigMetadata(name);

	if (data != nullptr && name != m_name)
	{
		if (m_ammo.find(name) == m_ammo.end())
		{
			m_ammo[name] = { data->clipSize, data->clipSize };
		}
		m_clipSize = data->clipSize;
		m_gunOffset = data->gunOffset;
		m_barrelOffset = data->barrelOffset;
		m_rigOffset = data->rigOffset;
		m_name = data->name;
		m_shootingAnimationSpeedup = data->shootingAnimationSpeedup;
		m_damage = data->damage;

		m_hands->setModel(ResourceManager::Instance().getAnimatedModel(data->name));
		m_gun->setModel(ResourceManager::Instance().getAnimatedModel(data->name + "_gun"));
		m_gunSound = std::make_unique<SourceVoice>( ResourceManager::Instance().getAudioFile(data->name), xaudio);

		m_hands->setFallbackAnimation("idle");
		m_gun->setFallbackAnimation("idle");

		// if there is no draw anmiation it does nothing
		m_hands->setAnimation("draw");
		m_gun->setAnimation("draw");
	}
}



void GunRig::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	this->m_hands->Render(renderMaster);
	this->m_gun->Render(renderMaster);
}

DirectX::XMFLOAT3 GunRig::GetBarrelOffset()
{
	return m_barrelOffset;
}

void GunRig::RotateAndOffset(DirectX::XMFLOAT3 yawPitchRoll, DirectX::XMFLOAT3 playerPos, float dt)
{
	DirectX::XMVECTOR rig_offset = DirectX::XMLoadFloat3(&m_rigOffset);

	DirectX::XMFLOAT3 actualRotation = { -yawPitchRoll.y, yawPitchRoll.x + XM_PI, 0 };
	this->m_gun->setRotation(actualRotation);
	this->m_hands->setRotation(actualRotation);

	DirectX::XMVECTOR new_offset = DirectX::XMLoadFloat3(&m_gunOffset);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(actualRotation.x, actualRotation.y, 0);
	new_offset = DirectX::XMVector3Transform(new_offset, rotationMatrix);
	rig_offset = DirectX::XMVector3Transform(rig_offset, rotationMatrix);

	auto rigPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&playerPos), rig_offset);
	new_offset = DirectX::XMVectorAdd(new_offset, rigPos);


	DirectX::XMFLOAT3 new_offset_f3;
	DirectX::XMStoreFloat3(&new_offset_f3, new_offset);

	this->m_gun->setPosition(new_offset_f3);

	DirectX::XMFLOAT3 rig_offset_f3;
	DirectX::XMStoreFloat3(&rig_offset_f3, rigPos);


	this->m_hands->setPosition(rig_offset_f3);

	// set up the barrel offset
	DirectX::XMVECTOR newBarrelOffset = DirectX::XMLoadFloat3(&m_initialBarrelOffset);
	newBarrelOffset = DirectX::XMVector3Transform(newBarrelOffset, rotationMatrix);
	DirectX::XMStoreFloat3(&m_barrelOffset, DirectX::XMVectorAdd(newBarrelOffset, { playerPos.x, playerPos.y, playerPos.z, 0.f }));

	this->Update(dt);
}
