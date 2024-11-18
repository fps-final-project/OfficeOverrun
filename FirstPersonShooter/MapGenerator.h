#pragma once
#include "RoomLayoutGenerator.h"

namespace WorldGenerator
{
	class __declspec(dllexport) MapGenerator
	{
	public:
		RoomLayout GenerateRoomLayout(RoomLayoutConfig config);
	};
}