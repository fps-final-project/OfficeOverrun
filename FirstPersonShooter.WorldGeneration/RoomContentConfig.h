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
		// Probability that the ak or sniper gun is sniper
		static constexpr float SNIPER_PROBABILITY = 0.3f;

		// Props count
		static const int PROPS_IN_ROOM_MIN = 0;
		static const int PROPS_IN_ROOM_MAX = 10;
		static const int PROPS_IN_ROOM_SIZE_TO_COUNT_COEFF = 8;
		// Prop placement
		static constexpr double PROPS_IN_ROOM_BINOMIAL_P = 0.5;
		static constexpr float MIN_PROP_OFFSET = 0.1;

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