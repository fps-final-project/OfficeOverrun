#include "pch.h"
#include "BinaryRoom.h"

using namespace WorldGenerator;

bool BinaryRoom::IsLeaf()
{
    return leftRoom == nullptr && rightRoom == nullptr;
}

void BinaryRoom::Split(RoomLayout& layout)
{
	if (width <= RoomLayoutConfig::maxRoom2DSize && depth <= RoomLayoutConfig::maxRoom2DSize && height <= RoomLayoutConfig::maxRoomHeight) // Room is of valid size
	{
		layout.rooms.push_back(MakeRoom());
		return;
	}

	int cutType = RngUtils::RandIntInRange(0,2);

	if (cutType == 2)
		SplitVertical(layout);
	else
		Split2D(layout, cutType);
}

void BinaryRoom::Split2D(RoomLayout& layout, int cutType)
{
	bool inDepth = (cutType == 1);

	// If 2D valid force vertical
	if (width <= RoomLayoutConfig::maxRoom2DSize && depth <= RoomLayoutConfig::maxRoom2DSize)
	{
		SplitVertical(layout);
		return;
	}

	// If valid in one direction force the other one
	if (width <= RoomLayoutConfig::maxRoom2DSize)
	{
		inDepth = false;
	}
	if (depth <= RoomLayoutConfig::maxRoom2DSize)
	{
		inDepth = true;
	}

	int cutOffset = RngUtils::RandIntInRange(RoomLayoutConfig::minRoom2DSize, (inDepth ? width : depth) - RoomLayoutConfig::minRoom2DSize);

	leftRoom = new BinaryRoom(x, y, z, inDepth ? cutOffset : width, inDepth ? depth : cutOffset, height);
	rightRoom = new BinaryRoom(inDepth ? x + cutOffset : x, inDepth ? y : y + cutOffset, z, inDepth ? width - cutOffset : width, inDepth ? depth : depth - cutOffset, height);

	leftRoom->Split(layout);
	rightRoom->Split(layout);
}

void BinaryRoom::SplitVertical(RoomLayout& layout)
{
	// Valid force random 2D
	if (height <= RoomLayoutConfig::maxRoomHeight)
	{
		Split2D(layout, RngUtils::RandIntInRange(0,1));
		return;
	}

	int cutOffset = RngUtils::RandIntInRange(RoomLayoutConfig::minRoomHeight, height - RoomLayoutConfig::minRoomHeight);

	leftRoom = new BinaryRoom(x, y, z, width, depth, cutOffset);
	rightRoom = new BinaryRoom(x, y, z + cutOffset, width, depth, height - cutOffset);

	leftRoom->Split(layout);
	rightRoom->Split(layout);
}



BinaryRoom::BinaryRoom(int x, int y, int z, int width, int depth, int height): x(x), y(y), z(z), width(width), depth(depth), height(height)
{
}

BinaryRoom BinaryRoom::GenerateRootRoom(Vector3 size)
{
	return BinaryRoom(0, 0, 0, size.x, size.y, size.z);
}

GeneratedRoom BinaryRoom::MakeRoom()
{
	GeneratedRoom room;
	room.pos = Vector3(x, y, z);
	room.size = Vector3(width, depth, height);

	return room;
}
