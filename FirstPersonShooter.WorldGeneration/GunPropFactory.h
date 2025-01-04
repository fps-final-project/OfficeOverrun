#pragma once
#include "GunPropInstance.h"
#include "GunType.h"

namespace WorldGenerator
{
	class GunPropFactory
	{
	private:
		static constexpr DirectX::XMFLOAT3 GUN_PROP_SIZE = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	public:
		static GunProp* CreateGunProp(GunType gun_type);
	};
}