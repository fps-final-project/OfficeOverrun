#include "pch.h"
#include "GunPropInstanceFactory.h"

using namespace WorldGenerator;

GunPropInstance* GunPropInstanceFactory::CreateGunPropInstance(GunType gun_type, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
{
	switch (gun_type)
	{
	case AK:
		return new GunPropInstance("ak_gun", "ak", position, GUN_PROP_SIZE, rotation, false);
	case Smg:
		return new GunPropInstance("smg_gun", "smg", position, GUN_PROP_SIZE, rotation, false);
	case Sniper:
		return new GunPropInstance("sniper_gun", "sniper", position, GUN_PROP_SIZE, rotation, false);
	}
}
