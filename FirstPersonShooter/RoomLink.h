#pragma once

#include "Vector3.h"
#include "GeneratedRoom.h"
namespace WorldGenerator
{
	// the 2 dimensions that link is defined at
	enum Orientation {
		YZ,
		XZ,
		XY
	};

	class GeneratedRoom;

	class RoomLink
	{
	public:
		int linkedRoomIdx; // index to linked room in room collection
		Vector3 pos;
		Orientation orientation;

		RoomLink(const Vector3& pos, const Orientation& orientation);

		static RoomLink MakeRoomLink(Vector3 pos, Vector3 size);
		static bool ValidBorderForRoomLink(Vector3 size);
	};
}