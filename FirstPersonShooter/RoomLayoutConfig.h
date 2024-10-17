#pragma once
namespace WorldGenerator
{
	struct RoomLayoutConfig
	{
		// For BSP 
		const static int minRoom2DSize = 4;
		const static int maxRoom2DSize = 12;
		const static int minRoomHeight = 4;
		const static int maxRoomHeight = 8;

		Vector3 mapSize;
	};
}