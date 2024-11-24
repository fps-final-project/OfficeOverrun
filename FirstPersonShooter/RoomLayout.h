#pragma once

#include <vector>
#include "GeneratedRoom.h"
namespace WorldGenerator
{
	struct __declspec(dllexport) RoomLayout
	{
		Vector3 mapSize;
		std::vector<GeneratedRoom> rooms;

		RoomLayout(Vector3 mapSize) : mapSize(mapSize)
		{
		}
	};
}