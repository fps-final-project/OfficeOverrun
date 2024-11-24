#include "pch.h"
#include <iostream>
#include "MapGenerator.h"
#include "RNGEngine.h"

#define SEED 432423

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRoomLayout(RoomLayoutConfig config)
{
	RNGEngine::GetInstance()->SetSeed(SEED);
	RoomLayoutGenerator generator(config);

	// Step 1
	generator.GenerateRooms();

	// Step 2
	generator.GenerateAdGraph();

	// Step 3
	generator.SelectRooms();

	// Step 4
	generator.GenerateRoomLinks();

	// Step 5
	generator.GenerateLayoutFromAdGraph();

	return generator.GetLayout();
}
