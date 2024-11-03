#include "pch.h"
#include "MappingHelpers.h"
#include "RoomLayoutConfig.h"

using namespace WorldGenerator;


Vector3 MappingHelpers::PositionToGameOrientation(Vector3 v)
{
	return Vector3(v.x, v.z, v.y);
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