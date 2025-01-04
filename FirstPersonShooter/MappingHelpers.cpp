#include "pch.h"
#include "MappingHelpers.h"
#include "RoomLayoutConfig.h"

using namespace WorldGenerator;


Vector3 MappingHelpers::PositionToGameOrientation(Vector3 v)
{
	return Vector3(v.x, v.z, v.y);
}

DirectX::XMFLOAT3 MappingHelpers::PositionToGameOrientation(DirectX::XMFLOAT3 v)
{
	return DirectX::XMFLOAT3(v.x, v.z, v.y);
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

PropInstance MappingHelpers::MapPropInstance(PropInstance prop)
{
	prop.position = PositionToGameOrientation(prop.position);
	prop.position.y = FloorToHeight(prop.position.y);
	prop.size = PositionToGameOrientation(prop.size);
	prop.rotation = PositionToGameOrientation(prop.rotation);
	//prop.rotation = DirectX::XMFLOAT3(0, DirectX::XM_PI, 0);
	return prop;
}

GunPropInstance MappingHelpers::MapGunPropInstance(GunPropInstance prop)
{
	prop.position = PositionToGameOrientation(prop.position);
	prop.position.y = FloorToHeight(prop.position.y);
	prop.size = PositionToGameOrientation(prop.size);
	prop.rotation = PositionToGameOrientation(prop.rotation);

	// Gun needs to be floating
	prop.position.y += 0.5f;

	return prop;
}
