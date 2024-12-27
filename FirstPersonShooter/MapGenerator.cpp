#include "pch.h"
#include <iostream>
#include "MapGenerator.h"
#include "RoomTypesGenerator.h"

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRooms(RoomLayoutConfig config, int seed)
{
	RNGEngine::GetInstance()->SetSeed(seed);
	RoomLayoutGenerator layout_generator(config);
	RoomTypesGenerator room_types_generator;

	// Step 1
	RoomLayout layout = layout_generator.GenerateRooms();

	// Step 2
	Graph<GeneratedRoom> adGraph = layout_generator.GenerateAdGraph(layout);

	// Step 3
	layout_generator.SelectRooms(adGraph);

	// Generate room types
	room_types_generator.GenerateRoomTypes(adGraph);

	// Step 4
	layout_generator.GenerateRoomLinks(adGraph);

	// Step 5
	layout = layout_generator.GenerateLayoutFromAdGraph(adGraph);

	return layout;
}
