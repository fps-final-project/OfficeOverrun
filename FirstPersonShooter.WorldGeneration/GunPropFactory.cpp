#include "pch.h"
#include "GunPropFactory.h"

using namespace WorldGenerator;

GunProp* GunPropFactory::CreateGunProp(GunType gun_type)
{
	switch (gun_type)
	{
	case AK:
		return new GunProp("ak_gun", "ak", GUN_PROP_SIZE, false);
	case Smg:
		return new GunProp("smg_gun", "smg", GUN_PROP_SIZE, false);
	case Sniper:
		return new GunProp("sniper_gun", "sniper", GUN_PROP_SIZE, false);
	default:
		return nullptr;
	}
}
