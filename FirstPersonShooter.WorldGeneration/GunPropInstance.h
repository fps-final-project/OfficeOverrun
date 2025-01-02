#pragma once

#include "PropInstance.hpp"
#include "GunProp.h"

namespace WorldGenerator
{
	struct __declspec(dllexport) GunPropInstance : PropInstance
	{
		std::string model_name;

		GunPropInstance(GunProp prop, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
			: model_name(prop.model_name), PropInstance(prop, position, rotation)
		{

		}
	};
}