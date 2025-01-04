#pragma once
#include <vector>
#include "Prop.hpp"
#include "MeshBox.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class PropMeshGenerator
	{
	public:
		static std::vector<MeshBox> GenerateMeshForProp(GeneratedRoom room, Prop prop);
		static void DeleteUnavailableBoxes(std::vector<MeshBox>& mesh, GeneratedRoom room);
		static DirectX::XMFLOAT2 PlacePropInBox(MeshBox box, DirectX::XMFLOAT3 prop_size);
	private:
		static bool BoxUnavailable(MeshBox box, const GeneratedRoom &room);
		static bool BoxIntestectsWithProp(MeshBox box, const PropInstance &prop);
		static bool BoxIntestectsWithRoomLink(const GeneratedRoom &room, MeshBox box, const RoomLink &link);
	};
}

