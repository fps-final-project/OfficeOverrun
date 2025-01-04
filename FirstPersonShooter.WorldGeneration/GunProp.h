#pragma once

#include "Prop.hpp"

namespace WorldGenerator
{
	struct __declspec(dllexport) GunProp : Prop
	{
		std::string model_name;

		GunProp(const std::string& model_name, std::string name, DirectX::XMFLOAT3 size, bool isSolid)
			: model_name(model_name)
		{
			this->name = name;
			this->size = size;
			this->isSolid = isSolid;
			this->faceVector = DirectX::XMFLOAT3(0, 1, 0);
		}
	};
}