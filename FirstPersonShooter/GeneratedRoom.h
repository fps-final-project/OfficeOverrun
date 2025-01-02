#pragma once

#include <tuple>
#include <vector>
#include "Vector3.h"
#include "RoomLink.h"
#include "GeometryUtils.h"
#include "../FirstPersonShooter.WorldGeneration/GunPropInstance.h"

namespace WorldGenerator
{
	class RoomLink;

	class __declspec(dllexport) GeneratedRoom
	{
	private:
		bool IsAdjacent(GeneratedRoom room);
	public:
		Vector3 pos; // minimal [x,y,z] coordinates of the room
		Vector3 size;
		std::vector<RoomLink> links; // links to other rooms
		int enemies;
		std::vector<PropInstance> props;
		std::vector<GunPropInstance> gun_props;

		bool ValidRoomLink(GeneratedRoom room);
		bool IsAbove(GeneratedRoom room);
		bool IsBelow(GeneratedRoom room);
		bool IsSameLevel(GeneratedRoom room);
		bool IsZeroFloor();
		std::tuple<Vector3, Vector3> ComputeBorders(GeneratedRoom room); // Returns pos and size
	};
}