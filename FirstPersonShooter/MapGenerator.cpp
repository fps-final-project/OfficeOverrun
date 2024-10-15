#include "pch.h"
#include <iostream>
#include "MapGenerator.h"
#include "RNG.h"

#define SEED 432423

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRoomLayout(RoomLayoutConfig config)
{
	RNG::GetInstance()->SetSeed(SEED);
	RoomLayoutGenerator generator(config);

	RoomLayout layout = generator.Generate();

	return layout;
}
