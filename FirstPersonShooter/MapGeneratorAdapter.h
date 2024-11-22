#pragma once
#include <vector>
#include "MapGenerator.h"
#include "MapGeneratorMapper.h"

using namespace WorldGenerator;

class  __declspec(dllexport) MapGeneratorAdapter
{
private:
	RoomLayoutConfig roomLayoutConfig;
	static RoomLayoutConfig MakeDefaultLayoutConfig();
public:
	MapGeneratorAdapter();
	void WithConfig(RoomLayoutConfig config);
	std::vector<Room> GenerateRooms();
};