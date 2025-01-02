#pragma once
#include <vector>
#include "Prop.hpp"
#include "MeshBox.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class PropMeshGenerator
	{
		static bool BoxUnavailable(MeshBox box, const GeneratedRoom &room);
	public:
		static std::vector<MeshBox> GenerateMeshForProp(GeneratedRoom room, Prop prop);
		static void DeleteUnavailableBoxes(std::vector<MeshBox>& mesh, GeneratedRoom room);
		static DirectX::XMFLOAT2 PlacePropInBox(MeshBox box, const Prop& prop);
	};
}

