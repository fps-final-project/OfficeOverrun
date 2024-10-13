#include "pch.h"
#include <iostream>
#include "RoomLayoutGenerator.h"
#include "RNG.h"

#define SEED 432423

RoomLayoutConfig GetLayoutConfig()
{
	RoomLayoutConfig config;
	config.mapSize = Vector3(10, 10, 3);
	config.roomCount = 10;
	return config;
}

int testAPI()
{
	RNG::GetInstance()->SetSeed(SEED);
	RoomLayoutGenerator generator(GetLayoutConfig());

	RoomLayout layout = generator.Generate();

	return 2137;
}

