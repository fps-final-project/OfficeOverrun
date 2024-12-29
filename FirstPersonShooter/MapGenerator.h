#pragma once
#include "RoomLayoutGenerator.h"
#include "RNGEngine.h"
#include "WorldGeneratorConfig.h"
#include "../FirstPersonShooter.WorldGeneration/RoomTypesGenerator.h"

namespace WorldGenerator
{
	class __declspec(dllexport) MapGenerator
	{
	public:
		RoomLayout GenerateRooms(WorldGeneratorConfig config, int seed);
	};
}