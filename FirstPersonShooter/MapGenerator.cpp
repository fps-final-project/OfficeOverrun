#include "pch.h"
#include <iostream>
#include "MapGenerator.h"

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRooms(RoomLayoutConfig config, int seed)
{
	RNGEngine::GetInstance()->SetSeed(seed);
	RoomLayoutGenerator generator(config);

	// Step 1
	RoomLayout layout = generator.GenerateRooms();

	// Step 2
	Graph<GeneratedRoom> adGraph = generator.GenerateAdGraph(layout);

	// Step 3
	generator.SelectRooms(adGraph);

	// Step 4
	generator.GenerateRoomLinks(adGraph);

	// Step 5
	layout = generator.GenerateLayoutFromAdGraph(adGraph);

	return layout;
}
