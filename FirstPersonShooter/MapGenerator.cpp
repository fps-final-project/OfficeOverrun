#include "pch.h"
#include <iostream>
#include "MapGenerator.h"

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRooms(WorldGeneratorConfig config, int seed)
{
	RNGEngine::GetInstance()->SetSeed(seed);
	RoomLayoutGenerator layout_generator(config.room_layout_config);
	RoomTypesGenerator room_types_generator(config.room_types_config);

	// Generate physical layout
	RoomLayout layout = layout_generator.GenerateRooms();

	// Generate adjacency graph based on the layout
	Graph<GeneratedRoom> adGraph = layout_generator.GenerateAdGraph(layout);

	// Reduce the graph
	layout_generator.SelectRooms(adGraph);

	// Generate room types
	room_types_generator.GenerateRoomTypes(adGraph);

	// Generate roomlink objects with respect to the graph structure
	layout_generator.GenerateRoomLinks(adGraph);

	// Generate physical layout base on the modified adjacency graph
	layout = layout_generator.GenerateLayoutFromAdGraph(adGraph);

	return layout;
}
