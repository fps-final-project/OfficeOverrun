#include "pch.h"
#include <iostream>
#include "MapGenerator.h"

using namespace WorldGenerator;

RoomLayout MapGenerator::GenerateRooms(WorldGeneratorConfig config, int seed)
{
	RNGEngine::GetInstance()->SetSeed(seed);
	RoomLayoutGenerator layout_generator(config.room_layout_config);
	RoomTypesGenerator room_types_generator(config.room_types_config);
	RoomContentGenerator room_content_generator(config.room_content_config);

	// Generate physical layout
	RoomLayout layout = layout_generator.GenerateRooms();

	// Generate adjacency graph based on the layout
	Graph<GeneratedRoom> adGraph = layout_generator.GenerateAdGraph(layout);

	// Reduce the graph
	layout_generator.SelectRooms(adGraph);

	// Set default room labels on adjacency graph nodes
	room_types_generator.SetDefaultNodeLabels(adGraph);

	// Generate room types
	room_types_generator.GenerateRoomTypes(adGraph);

	// Generate enemies in rooms
	room_types_generator.GenerateEnemies(adGraph);

	// Generate roomlink objects with respect to the graph structure
	layout_generator.GenerateRoomLinks(adGraph);

	// Generate props in the rooms
	room_content_generator.GenerateRoomContent(adGraph);

	// Generate physical layout base on the modified adjacency graph
	layout = layout_generator.GenerateLayoutFromAdGraph(adGraph);

	return layout;
}
