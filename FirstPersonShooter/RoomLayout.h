#pragma once

#include <vector>
#include "GeneratedRoom.h"

struct RoomLayout
{
	Vector3 mapSize;
	std::vector<GeneratedRoom> rooms;

	RoomLayout(Vector3 mapSize): mapSize(mapSize)
	{
	}
};