#pragma once

#include "Vector3.h"
#include "GeneratedRoom.h"

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
	GeneratedRoom* linkedRoom;
	Vector3 pos;
	Orientation orientation;

	RoomLink(const Vector3& pos, const Orientation& orientation, GeneratedRoom* linkedRoom);

	static RoomLink MakeRoomLink(Vector3 pos, Vector3 size, GeneratedRoom* linkedRoom);
};