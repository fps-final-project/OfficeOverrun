#include "pch.h"
#include "MappingHelpers.h"
#include "RoomLayoutConfig.h"
#include "GeometryUtils.h"

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
	prop.position = GeometryUtils::AdjustPropPositionToOrientation(prop.rotation, prop.size, prop.position);
	prop.position = PositionToGameOrientation(prop.position);
	prop.position.y = FloorToHeight(prop.position.y);
	prop.size = PositionToGameOrientation(prop.size);
	prop.rotation = PositionToGameOrientation(prop.rotation);

	prop.AABB_position = PositionToGameOrientation(prop.AABB_position);
	prop.AABB_position.y = prop.position.y;
	prop.AABB_size = PositionToGameOrientation(prop.AABB_size);
	//prop.rotation = DirectX::XMFLOAT3(0, DirectX::XM_PI, 0);
	return prop;
}

GunPropInstance MappingHelpers::MapGunPropInstance(GunPropInstance prop)
{
	prop.position = PositionToGameOrientation(prop.position);
	prop.position.y = FloorToHeight(prop.position.y);
	prop.rotation = PositionToGameOrientation(prop.rotation);

	// Prop box in world generator is two times bigger so gun would not rotate into walls and prop
	DirectX::XMFLOAT3 gunPropSize(0.5f, 0.5f, 0.5f);
	prop.position = DirectX::XMFLOAT3(prop.position.x + gunPropSize.x, prop.position.y + gunPropSize.y, prop.position.z + gunPropSize.z);
	prop.size = gunPropSize;

	return prop;
}
