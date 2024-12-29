#pragma once
#include <string>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomTypesConfig
	{
	private:
		const std::string PRODUCTIONS_DIR = R"(.\FirstPersonShooter_WorldGeneration\Grammar Configuration\Productions)";

	public:
		static const int ENEMY_IN_NORMAL_ROOM_MIN = 0;
		static const int ENEMY_IN_NORMAL_ROOM_MAX = 5;
		static const int ENEMY_IN_NORMAL_ROOM_POISSON_LAMBDA = 1;
		static const int ENEMY_IN_BOSS_ROOM_MIN = 6;
		static const int ENEMY_IN_BOSS_ROOM_MAX = 10;
		static const int ENEMY_IN_BOSS_ROOM_POISSON_LAMBDA = 8;

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