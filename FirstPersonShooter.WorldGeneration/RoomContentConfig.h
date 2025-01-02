#pragma once
#include <string>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomContentConfig
	{
		const std::string PROPS_DIR = R"(Assets/props)";
	public:
		std::string propsDir() { return PROPS_DIR; }

		static const int PROPS_IN_ROOM_MIN = 0;
		static const int PROPS_IN_ROOM_MAX = 10;
		static const int PROPS_IN_ROOM_SIZE_TO_COUNT_COEFF = 8;
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