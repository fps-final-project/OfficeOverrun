#pragma once

#include "RoomLayout.h"
#include "Utils.h"
#include "RoomLayoutConfig.h"

// credits: https://medium.com/@guribemontero/dungeon-generation-using-binary-space-trees-47d4a668e2d0
class BinaryRoom
{
public:
	int x, y, z; // the minimal coordinates cuboid vertex
	int width, depth, height;
	BinaryRoom* leftRoom;
	BinaryRoom* rightRoom;

	bool IsLeaf();
	void Split(RoomLayout& layout);
	void Split2D(RoomLayout& layout, int cutType);
	void SplitVertical(RoomLayout& layout);
	BinaryRoom(int x, int y, int z, int width, int depth, int height);
	static BinaryRoom GenerateRootRoom(Vector3 size);
private:
	GeneratedRoom MakeRoom();
};