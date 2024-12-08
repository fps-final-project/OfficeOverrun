#include "pch.h"
#include "MapGeneratorAdapter.h"
#include <algorithm>
#include <iterator>

RoomLayoutConfig MapGeneratorAdapter::MakeDefaultLayoutConfig()
{
	RoomLayoutConfig config;
	config.mapSize = Vector3(30, 30, 3);
	config.roomDensity = 0.6;
	config.pathLengthCoeff = 0.6;
	config.edgeDensityCoeff = 0.3;
	return config;
}

MapGeneratorAdapter::MapGeneratorAdapter()
{
	roomLayoutConfig = MakeDefaultLayoutConfig();
	seed = time(nullptr);
}

MapGeneratorAdapter& MapGeneratorAdapter::WithSeed(int seed)
{
	this->seed = seed;
	return *this;
}

std::vector<Room> MapGeneratorAdapter::GenerateRooms()
{
	RoomLayout layout = MapGenerator().GenerateRoomLayout(roomLayoutConfig, seed);
	std::vector<Room> rooms = std::vector<Room>();

	// Mapping function
	auto map = [](GeneratedRoom obj)
		{
			return MapGeneratorMapper::Map<GeneratedRoom, Room>(obj);
		};

	std::transform(layout.rooms.begin(), layout.rooms.end(), std::back_inserter(rooms), map);

	return rooms;
}
