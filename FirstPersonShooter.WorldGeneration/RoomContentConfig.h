#pragma once
#include <string>
#include <DirectXMath.h>

namespace WorldGenerator
{
	class __declspec(dllexport) RoomContentConfig
	{
		const std::string PROPS_DIR = R"(Assets/props)";
	public:
		std::string propsDir() { return PROPS_DIR; }

		// Enemy count selection
		static const int ENEMY_IN_NORMAL_ROOM_MIN = 0;
		static const int ENEMY_IN_NORMAL_ROOM_MAX = 3;
		static const int ENEMY_IN_NORMAL_ROOM_POISSON_LAMBDA = 1;
		static const int ENEMY_IN_BOSS_ROOM_MIN = 4;
		static const int ENEMY_IN_BOSS_ROOM_MAX = 7;
		static const int ENEMY_IN_BOSS_ROOM_POISSON_LAMBDA = 5;

		// Guns count selection
		// Number of ak or sniper in treasure room
		static const int MIN_AK_SNIPER_IN_TREASURE_ROOM = 1;
		static const int MAX_AK_SNIPER_IN_TREASURE_ROOM = 2;

		// Probability that the ak or sniper gun is sniper
		static constexpr float SNIPER_PROBABILITY = 0.35f;

		// Smgs in treasure room
		static const int MIN_SMGS_IN_TREASURE_ROOM = 2;
		static const int MAX_SMGS_IN_TREASURE_ROOM = 4;
		static constexpr float SMGS_IN_TREASURE_ROOM_BINOMIAL_P = 0.3f;
		static constexpr float SMGS_IN_TREASURE_ROOM_SIZE_TO_COUNT_COEFF = 0.15f;

		// Smgs in default room
		static const int MIN_SMGS_IN_DEFAULT_ROOM = 0;
		static const int MAX_SMGS_IN_DEFAULT_ROOM = 2;
		static constexpr float SMGS_IN_DEFAULT_ROOM_BINOMIAL_P = 0.1f;
		static constexpr float SMGS_IN_DEFAULT_ROOM_SIZE_TO_COUNT_COEFF = 0.10f;

		// Props count
		static const int PROPS_IN_ROOM_MIN = 0;
		static const int PROPS_IN_ROOM_MAX = 10;
		static constexpr float PROPS_IN_ROOM_SIZE_TO_COUNT_COEFF = 0.125f;
		// Prop placement
		static constexpr double PROPS_IN_ROOM_BINOMIAL_P = 0.5f;
		static constexpr float MIN_PROP_OFFSET = 0.1f;

		static constexpr DirectX::XMFLOAT3 RESP_POINT = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		static constexpr DirectX::XMFLOAT2 RESP_POINT_BOX = DirectX::XMFLOAT2(2.f, 2.f);

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