#include "pch.h"
#include "MappingHelpers.h"

Vector3 MappingHelpers::PositionToGameOrientation(Vector3 v)
{
	return Vector3(v.x, v.y, v.z);
}
