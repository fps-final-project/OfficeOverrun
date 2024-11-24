#pragma once
#include "GeneratedRoom.h"

using namespace WorldGenerator;
namespace FirstPersonShooter_WorldGeneration_Test
{
	class TestUtils
	{
	public:
		static GeneratedRoom MakeGeneratedRoom(int posX, int posY, int posZ, int sizeX, int sizeY);
	};
}