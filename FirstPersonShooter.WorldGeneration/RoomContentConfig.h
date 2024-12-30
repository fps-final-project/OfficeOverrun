#pragma once
#include <string>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomContentConfig
	{
		const std::string PROPS_DIR = R"(Assets/props)";
	public:
		std::string propsDir() { return PROPS_DIR; }

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