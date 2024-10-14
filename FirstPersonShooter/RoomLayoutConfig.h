#pragma once
namespace WorldGenerator
{
	struct RoomLayoutConfig
	{
		// For BSP 
		const static int minRoom2DSize = 3;
		const static int maxRoom2DSize = 6;
		const static int minRoomHeight = 1;
		const static int maxRoomHeight = 2;

		Vector3 mapSize;
		int roomCount;
	};
}