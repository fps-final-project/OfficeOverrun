#pragma once

#include <tuple>
#include <vector>
#include "Vector3.h"
#include "RoomLink.h"

namespace WorldGenerator
{
	class RoomLink;

	class GeneratedRoom
	{
	public:
		Vector3 pos; // minimal [x,y,z] coordinates of the room
		Vector3 size;
		std::vector<RoomLink> links; // links to other rooms

		bool IsAdjacent(GeneratedRoom room);
		bool IsAbove(GeneratedRoom room);
		bool IsBelow(GeneratedRoom room);
		bool IsZeroFloor();
		std::tuple<Vector3, Vector3> ComputeBorders(GeneratedRoom room); // Returns pos and size
	};
}