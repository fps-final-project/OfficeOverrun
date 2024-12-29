#pragma once
#include <string>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomTypesConfig
	{
	private:
		const std::string PRODUCTIONS_DIR = R"(.\FirstPersonShooter_WorldGeneration\Grammar Configuration\Productions)";

	public:
		std::string productionsDir() { return PRODUCTIONS_DIR; }

		RoomTypesConfig()
		{
		}

		RoomTypesConfig(const RoomTypesConfig& other)
		{
		}

		RoomTypesConfig& operator=(const RoomTypesConfig& other)
		{
			return RoomTypesConfig(other);
		}
	};

}