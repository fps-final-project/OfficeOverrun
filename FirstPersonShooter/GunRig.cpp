#include "pch.h"
#include "GunRig.h"
#include <cmath>
#include "ResourceManager.h"
#include "SpriteRenderer.hpp"

GunRig::GunRig(std::string gunName, IXAudio2* xaudio) : 
	m_gunSound(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile(gunName), xaudio)), 
	m_reloadSound(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile("reload"), xaudio)),
	m_emptyClip(std::make_unique<SourceVoice>(ResourceManager::Instance().getAudioFile("empty-clip"), xaudio))

{
	m_hands = std::make_shared<AnimatedEntity>(ResourceManager::Instance().getAnimatedModel(gunName));
	m_gun = std::make_shared<AnimatedEntity>(ResourceManager::Instance().getAnimatedModel(gunName + "_gun"));
	 
	this->ChangeGun(gunName, xaudio);
}

void GunRig::Update(float dt)
{
	this->m_hands->Update(dt);
	this->m_gun->Update(dt);
	m_muzzleFlash.Update(dt);
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
	this->m_hands->setAnimation("shoot", m_shootingAnimationSpeedup, false);
	this->m_gun->setAnimation("shoot", m_shootingAnimationSpeedup, false);
	m_gunSound->PlaySound(true);
	m_muzzleFlash.PlayAnimation();
	return true;
}

void GunRig::CollectAmmo(const std::string& name, int amount)
{
	if (m_ammo.find(name) == m_ammo.end())
		return;
	
	m_ammo[name].second += amount;
}

void GunRig::ChangeGun(const std::string& name, IXAudio2* xaudio)
{
	std::shared_ptr<GunRigMetadata> data = ResourceManager::Instance().getGunRigMetadata(name);

	if (data != nullptr && name != m_name)
	{
		if (m_ammo.find(name) == m_ammo.end())
		{
			m_ammo[name] = { data->clipSize, data->clipSize * 4 };
		}
		m_clipSize = data->clipSize;
		m_gunOffset = data->gunOffset;
		m_rigOffset = data->rigOffset;
		m_initialBarrelOffset = data->barrelOffset;
		m_name = data->name;
		m_shootingAnimationSpeedup = data->shootingAnimationSpeedup;
		m_damage = data->damage;

		m_muzzleFlash.SetPosition(data->screenBarrelOffsetX, data->screenBarrelOffsetY);

		m_hands->setModel(ResourceManager::Instance().getAnimatedModel(data->name));
		m_gun->setModel(ResourceManager::Instance().getAnimatedModel(data->name + "_gun"));
		m_gunSound = std::make_unique<SourceVoice>( ResourceManager::Instance().getAudioFile(data->name), xaudio);
		m_gunSound->SetVolume(0.5f);

		m_hands->setFallbackAnimation("idle");
		m_gun->setFallbackAnimation("idle");

		// if there is no draw anmiation it does nothing
		m_hands->setAnimation("draw");
		m_gun->setAnimation("draw");
	}
}

void GunRig::Render(std::shared_ptr<RenderMaster> renderMaster)
{
	m_hands->Render(renderMaster);
	m_gun->Render(renderMaster);
}

void GunRig::RenderMuzzleFlash(std::shared_ptr<SpriteRenderer> spriteRenderer, int screenWidth, int screenHeight)
{
	m_muzzleFlash.Render(spriteRenderer, screenWidth, screenHeight);
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

	// set up the barrel offset
	DirectX::XMVECTOR newBarrelOffset = DirectX::XMLoadFloat3(&m_initialBarrelOffset);
	newBarrelOffset = DirectX::XMVector3Transform(newBarrelOffset, rotationMatrix);
	DirectX::XMStoreFloat3(&m_barrelOffset, DirectX::XMVectorAdd(newBarrelOffset, { playerPos.x, playerPos.y, playerPos.z, 0.f }));

	this->m_hands->setPosition(rig_offset_f3);

	this->Update(dt);
}
