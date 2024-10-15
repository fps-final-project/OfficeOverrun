#pragma once
#include "RoomLayoutGenerator.h"

namespace WorldGenerator
{
	class MapGenerator
	{
	public:
		RoomLayout GenerateRoomLayout(RoomLayoutConfig config);
	};
}