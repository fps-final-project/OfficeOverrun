#pragma once
#include "RoomLayoutGenerator.h"

namespace WorldGenerator
{
	class MapGenerator
	{
	private:
		static RoomLayoutConfig MakeTestLayoutConfig();
	public:
		RoomLayout GenerateRoomLayout(RoomLayoutConfig config = MakeTestLayoutConfig());
	};
}