#pragma once
#include "RoomLayoutConfig.h"
#include "RoomTypesConfig.h"

namespace WorldGenerator
{
	struct __declspec(dllexport) WorldGeneratorConfig
	{
		RoomLayoutConfig room_layout_config;
		RoomTypesConfig room_types_config;
	};
}