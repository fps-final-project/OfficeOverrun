#include "pch.h"
#include "BinaryRoom.h"

using namespace WorldGenerator;

void WorldGenerator::BinaryRoom::MakeRoomsOnLayoutFloor(RoomLayout& layout, int floor)
{
	BinaryRoom rootRoom = BinaryRoom(0, 0, floor, layout.mapSize.x, layout.mapSize.y, 1);
	rootRoom.Split(layout);
}

void BinaryRoom::Split(RoomLayout& layout)
{
	if (width <= RoomLayoutConfig::MAX_ROOM_2D_SIZE && depth <= RoomLayoutConfig::MAX_ROOM_2D_SIZE) // Room is of valid size
	{
		layout.rooms.push_back(MakeRoom());
		return;
	}

	Split2D(layout, RNG::RandIntInRange(0, 1));
}

void BinaryRoom::Split2D(RoomLayout& layout, int cutType)
{
	bool inDepth = (cutType == 1);

	// If valid in one direction force the other one
	if (width <= RoomLayoutConfig::MAX_ROOM_2D_SIZE)
	{
		inDepth = false;
	}
	if (depth <= RoomLayoutConfig::MAX_ROOM_2D_SIZE)
	{
		inDepth = true;
	}

	int cutOffset = RNG::RandIntInRange(RoomLayoutConfig::MIN_ROOM_2D_SIZE, (inDepth ? width : depth) - RoomLayoutConfig::MIN_ROOM_2D_SIZE);

	leftRoom = new BinaryRoom(x, y, z, inDepth ? cutOffset : width, inDepth ? depth : cutOffset, height);
	rightRoom = new BinaryRoom(inDepth ? x + cutOffset : x, inDepth ? y : y + cutOffset, z, inDepth ? width - cutOffset : width, inDepth ? depth : depth - cutOffset, height);

	leftRoom->Split(layout);
	rightRoom->Split(layout);
}


BinaryRoom::BinaryRoom(int x, int y, int z, int width, int depth, int height): x(x), y(y), z(z), width(width), depth(depth), height(height)
{
}

GeneratedRoom BinaryRoom::MakeRoom()
{
	GeneratedRoom room;
	room.pos = Vector3(x, y, z);
	room.size = Vector3(width, depth, height);

	return room;
}
