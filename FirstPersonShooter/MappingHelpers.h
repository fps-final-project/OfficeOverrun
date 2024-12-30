#pragma once
#include "Vector3.h"
#include "RoomLinkData.hpp"
#include "../FirstPersonShooter.WorldGeneration/PropInstance.hpp"

using namespace WorldGenerator;

class MappingHelpers
{
private:
	static int FloorToHeight(int floor);
	static Vector3 PositionToGameOrientation(Vector3 v);
	static DirectX::XMFLOAT3 PositionToGameOrientation(DirectX::XMFLOAT3 v);
public:
	static DirectX::XMFLOAT3 OrientationToSize(OrientationData orientation);
	static Vector3 MapVector(Vector3 v);
	static PropInstance MapPropInstance(PropInstance prop);
};

