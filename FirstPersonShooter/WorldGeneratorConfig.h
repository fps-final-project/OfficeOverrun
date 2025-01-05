#pragma once
#include "RoomLayoutConfig.h"
#include "../FirstPersonShooter.WorldGeneration/RoomTypesConfig.h"
#include "../FirstPersonShooter.WorldGeneration/RoomContentConfig.h"

namespace WorldGenerator
{
	class __declspec(dllexport) WorldGeneratorConfig
	{
	public:
		RoomLayoutConfig room_layout_config;
		RoomTypesConfig room_types_config;
		RoomContentConfig room_content_config;

		WorldGeneratorConfig(const WorldGeneratorConfig& other)
			: room_layout_config(other.room_layout_config), room_types_config(other.room_types_config), room_content_config(other.room_content_config)
		{
		}

		WorldGeneratorConfig& operator=(const WorldGeneratorConfig& other)
		{
			return WorldGeneratorConfig(other);
		}

		WorldGeneratorConfig(const RoomLayoutConfig& room_layout_config, const RoomTypesConfig& room_types_config, const RoomContentConfig& room_content_config)
			: room_layout_config(room_layout_config), room_types_config(room_types_config), room_content_config(room_content_config)
		{
		}

		WorldGeneratorConfig() = default;
	};
}