#include "pch.h"
#include <iostream>
#include "WorldGenerator.h"
#include "RNG.h"

#define SEED 432423

using namespace WorldGenerator;

RoomLayoutConfig GetLayoutConfig()
{
	RoomLayoutConfig config;
	config.mapSize = Vector3(10, 10, 3);
	config.roomCount = 10;
	return config;
}

RoomLayout WorldGenerator::testAPI()
{
	RNG::GetInstance()->SetSeed(SEED);
	RoomLayoutGenerator generator(GetLayoutConfig());

	RoomLayout layout = generator.Generate();

	return layout;
}
