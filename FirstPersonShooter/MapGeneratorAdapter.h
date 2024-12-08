#pragma once
#include <vector>
#include "MapGenerator.h"
#include "MapGeneratorMapper.h"

using namespace WorldGenerator;

class  __declspec(dllexport) MapGeneratorAdapter
{
private:
	RoomLayoutConfig roomLayoutConfig;
	int seed;
	static RoomLayoutConfig MakeDefaultLayoutConfig();
public:
	MapGeneratorAdapter();
	MapGeneratorAdapter& WithSeed(int seed);
	std::vector<Room> GenerateRooms();
};