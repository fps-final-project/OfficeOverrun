#pragma once

#include "PropInstance.hpp"

namespace WorldGenerator
{
	struct __declspec(dllexport) GunPropInstance : PropInstance
	{
		std::string model_name;

		GunPropInstance(const std::string& model_name, const std::string& name, 
			const DirectX::XMFLOAT3& position, DirectX::XMFLOAT3 size, const DirectX::XMFLOAT3& rotation, bool isSolid)
			: model_name(model_name), PropInstance(name, position, size, rotation, isSolid)
		{
		}
	};
}