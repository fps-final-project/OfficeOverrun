#pragma once
#include "Vector3.h"

using namespace WorldGenerator;

class MappingHelpers
{
private:
	static int FloorToHeight(int floor);
	static Vector3 PositionToGameOrientation(Vector3 v);
public:
	static Vector3 MapVector(Vector3 v);
};

