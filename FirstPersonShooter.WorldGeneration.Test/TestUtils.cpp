#include "pch.h"
#include "TestUtils.h"

using namespace WorldGenerator;
using namespace FirstPersonShooter_WorldGeneration_Test;

GeneratedRoom TestUtils::MakeGeneratedRoom(int posX, int posY, int posZ, int sizeX, int sizeY)
{
    GeneratedRoom room;
    room.pos = Vector3(posX, posY, posZ);
    room.size = Vector3(sizeX, sizeY, 1);

    return room;
}