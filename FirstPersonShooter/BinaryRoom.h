#pragma once

#include "RoomLayout.h"
#include "GeometryUtils.h"
#include "RNG.h"
#include "RoomLayoutConfig.h"

namespace WorldGenerator
{
	class __declspec(dllexport) BinaryRoom
	{
	public:
		int x, y, z; // the minimal coordinates cuboid vertex
		int width, depth, height;
		BinaryRoom* leftRoom;
		BinaryRoom* rightRoom;
		static void MakeRoomsOnLayoutFloor(RoomLayout& layout, int floor);
	private:
		void Split(RoomLayout& layout);
		void Split2D(RoomLayout& layout, int cutType);
		BinaryRoom(int x, int y, int z, int width, int depth, int height);
		GeneratedRoom MakeRoom();
	};
}