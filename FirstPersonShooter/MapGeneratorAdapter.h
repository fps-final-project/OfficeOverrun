#pragma once
#include <vector>
#include "MapGenerator.h"
#include "MapGeneratorMapper.h"
#include "../FirstPersonShooter.WorldGeneration/WorldGeneratorConfig.h"

using namespace WorldGenerator;

class  __declspec(dllexport) MapGeneratorAdapter
{
private:
	WorldGeneratorConfig config;
	int seed;
	static RoomLayoutConfig MakeDefaultRoomLayoutConfig();
	static RoomTypesConfig MakeDefaultRoomTypesConfig();
	static WorldGeneratorConfig MakeDefaultWorldGeneratorConfig();
public:
	MapGeneratorAdapter();
	MapGeneratorAdapter& WithSeed(int seed);
	std::vector<Room> GenerateRooms();
};