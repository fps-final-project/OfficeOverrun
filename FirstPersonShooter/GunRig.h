#pragma once
#include "AnimatedEntity.hpp"
#include <memory> 
#include "SourceVoice.hpp"
#include "MuzzleFlash.h"


class __declspec(dllexport) GunRig : public Drawable
{

public:
	GunRig(std::string gunName, IXAudio2* xaudio);

	virtual void Render(std::shared_ptr<RenderMaster> renderMaster);
	void RenderMuzzleFlash(std::shared_ptr<SpriteRenderer> spriteRenderer, int screenWidth, int screenHeight);
	
	void Update(float dt);
	void Reload();
	void ChangeGun(const std::string& name, IXAudio2* xaudio);
	void RotateAndOffset(DirectX::XMFLOAT3 yawPitchRoll, DirectX::XMFLOAT3 playerPos, float dt);
	bool Shoot();
	void CollectAmmo(const std::string& name, int amount);

	inline int GetDamage() { return m_damage; }
	inline bool IsIdle() { return m_hands->isIdle(); }
	inline bool IsMuzzleFlashOn() { return m_muzzleFlash.IsPlaying(); }
	DirectX::XMFLOAT3 GetBarrelOffset() { return m_barrelOffset; };
	inline int GetClipAmmo() { return m_ammo[m_name].first; }
	inline int GetTotalAmmo() { return m_ammo[m_name].second; }
private:
	DirectX::XMFLOAT3 m_rigOffset, m_gunOffset, m_initialBarrelOffset, m_barrelOffset;
	std::shared_ptr<AnimatedEntity> m_hands, m_gun;
	MuzzleFlash m_muzzleFlash;
	std::unique_ptr<SourceVoice> m_gunSound, m_reloadSound, m_emptyClip;
	std::string m_name;
	float m_shootingAnimationSpeedup;
	int m_damage;
	int m_clipSize;
	std::map<std::string, std::pair<int, int>> m_ammo;
};