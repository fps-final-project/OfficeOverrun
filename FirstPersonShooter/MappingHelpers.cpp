#include "pch.h"
#include "MappingHelpers.h"
#include "RoomLayoutConfig.h"

using namespace WorldGenerator;


Vector3 MappingHelpers::PositionToGameOrientation(Vector3 v)
{
	return Vector3(v.x, v.z, v.y);
}

DirectX::XMFLOAT3 MappingHelpers::OrientationToSize(OrientationData orientation)
{
	switch (orientation)
	{
	case XY:
		return DirectX::XMFLOAT3(1, 2, 0);
	case ZY:
		return DirectX::XMFLOAT3(0, 2, 1);
	case XZX:
		return DirectX::XMFLOAT3(RoomLayoutConfig::verticalRoomLinkLength, RoomLayoutConfig::roomHeight, RoomLayoutConfig::verticalRoomLinkWidth);
	default:
		return DirectX::XMFLOAT3(RoomLayoutConfig::verticalRoomLinkWidth, RoomLayoutConfig::roomHeight, RoomLayoutConfig::verticalRoomLinkLength);
	}
}

int MappingHelpers::FloorToHeight(int floor)
{
	return floor * RoomLayoutConfig::roomHeight;
}

Vector3 MappingHelpers::MapVector(Vector3 v)
{
	// Apply height
	v.z = FloorToHeight(v.z);
	// Apply orientation transform
	v = PositionToGameOrientation(v);

	return v;
}