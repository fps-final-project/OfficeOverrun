#pragma once
#include "GunPropInstance.h"
#include "GunType.h"

namespace WorldGenerator
{
	class GunPropInstanceFactory
	{
	private:
		static constexpr DirectX::XMFLOAT3 GUN_PROP_SIZE = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	public:
		static GunPropInstance* CreateGunPropInstance(GunType gun_type, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
	};
}