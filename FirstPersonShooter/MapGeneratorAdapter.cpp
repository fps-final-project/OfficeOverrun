#include "pch.h"
#include "MapGeneratorAdapter.h"
#include <algorithm>
#include <iterator>

RoomLayoutConfig MapGeneratorAdapter::MakeDefaultLayoutConfig()
{
	RoomLayoutConfig config;
	config.mapSize = Vector3(20, 20, 16);
	config.roomDensity = 1;
	config.pathLengthCoeff = 0.6;
	config.edgeDensityCoeff = 0.3;
	return config;
}

MapGeneratorAdapter::MapGeneratorAdapter()
{
	roomLayoutConfig = MakeDefaultLayoutConfig();
}

void MapGeneratorAdapter::WithConfig(RoomLayoutConfig config)
{
	roomLayoutConfig = config;
}

std::vector<Room> MapGeneratorAdapter::GenerateRooms()
{
	RoomLayout layout = MapGenerator().GenerateRoomLayout(roomLayoutConfig);
	std::vector<Room> rooms = std::vector<Room>();

	// Mapping function
	auto map = [](GeneratedRoom obj)
		{
			return MapGeneratorMapper::Map<GeneratedRoom, Room>(obj);
		};

	std::transform(layout.rooms.begin(), layout.rooms.end(), std::back_inserter(rooms), map);

	return rooms;
}
