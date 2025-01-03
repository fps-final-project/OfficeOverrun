#pragma once
#include <string>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomContentConfig
	{
		const std::string PROPS_DIR = R"(Assets/props)";
	public:
		std::string propsDir() { return PROPS_DIR; }

		// Guns count selection
		// Number of ak or sniper in treasure room
		static const int MIN_AK_SNIPER_IN_TREASURE_ROOM = 1;
		static const int MAX_AK_SNIPER_IN_TREASURE_ROOM = 3;

		// Smgs in treasure room
		static const int MIN_SMGS_IN_TREASURE_ROOM = 2;
		static const int MAX_SMGS_IN_TREASURE_ROOM = 8;
		static constexpr float SMGS_IN_TREASURE_ROOM_BINOMIAL_P = 0.5f;
		static constexpr float SMGS_IN_TREASURE_ROOM_SIZE_TO_COUNT_COEFF = 0.15f;

		// Smgs in default room
		static const int MIN_SMGS_IN_DEFAULT_ROOM = 0;
		static const int MAX_SMGS_IN_DEFAULT_ROOM = 5;
		static constexpr float SMGS_IN_DEFAULT_ROOM_BINOMIAL_P = 0.1f;
		static constexpr float SMGS_IN_DEFAULT_ROOM_SIZE_TO_COUNT_COEFF = 0.15f;
		// Probability that the ak or sniper gun is sniper
		static constexpr float SNIPER_PROBABILITY = 0.3f;

		// Props count
		static const int PROPS_IN_ROOM_MIN = 0;
		static const int PROPS_IN_ROOM_MAX = 10;
		static constexpr float PROPS_IN_ROOM_SIZE_TO_COUNT_COEFF = 0.125f;
		// Prop placement
		static constexpr double PROPS_IN_ROOM_BINOMIAL_P = 0.5f;
		static constexpr float MIN_PROP_OFFSET = 0.1f;

		RoomContentConfig()
		{
		}

		RoomContentConfig(const RoomContentConfig& other)
		{
		}

		RoomContentConfig& operator=(const RoomContentConfig& other)
		{
			return RoomContentConfig(other);
		}
	};
}