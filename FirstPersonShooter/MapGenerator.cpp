#include "pch.h"
#include <iostream>
#include "MapGenerator.h"
#include "RNG.h"

#define SEED 432423

using namespace WorldGenerator;

RoomLayoutConfig MapGenerator::MakeTestLayoutConfig()
{
	RoomLayoutConfig config;
	config.mapSize = Vector3(10, 10, 3);
	return config;
}

RoomLayout MapGenerator::GenerateRoomLayout(RoomLayoutConfig config)
{
	RNG::GetInstance()->SetSeed(SEED);
	RoomLayoutGenerator generator(config);

	RoomLayout layout = generator.Generate();

	return layout;
}
