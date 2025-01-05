#pragma once

#include "PropInstance.hpp"
#include "GunProp.h"

namespace WorldGenerator
{
	struct __declspec(dllexport) GunPropInstance : PropInstance
	{
		std::string model_name;

		GunPropInstance(GunProp prop, PropInstance prop_instance)
			: model_name(prop.model_name), PropInstance(prop_instance)
		{

		}
	};
}