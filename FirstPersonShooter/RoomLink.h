#pragma once

#include "Vector3.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	// the 2 dimensions that link is defined at
	enum __declspec(dllexport) Orientation {
		YZ,
		XZ,
		XYX, // X is length
		XYY // Y is length
	};

	class GeneratedRoom;

	class __declspec(dllexport) RoomLink
	{
	private:
		static Orientation MakeOrientation(Vector3 size);
	public:
		int linkedRoomIdx; // index to linked room in room collection
		Vector3 pos;
		Orientation orientation;

		RoomLink(const Vector3& pos, const Orientation& orientation);

		static RoomLink MakeRoomLink(Vector3 pos, Vector3 size);
		static bool ValidSizeForRoomLink(Vector3 size);
	};
}