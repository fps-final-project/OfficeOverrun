#pragma once
#include "RoomLayoutGenerator.h"
#include "RNGEngine.h"

namespace WorldGenerator
{
	class __declspec(dllexport) MapGenerator
	{
	public:
		RoomLayout GenerateRooms(RoomLayoutConfig config, int seed);
	};
}