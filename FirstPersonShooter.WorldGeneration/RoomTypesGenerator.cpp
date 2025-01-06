#include "pch.h"
#include "RoomTypesGenerator.h"
#include "GraphGrammar.h";
#include "GraphGrammarReader.h";
#include "RoomLabel.h"
#include "RoomGraphExtensions.h"
#include "EnemyCountSelector.h"
#include "GraphUtils.h"

using namespace WorldGenerator;

void WorldGenerator::RoomTypesGenerator::SetDefaultNodeLabel(Graph<GeneratedRoom>& adGraph, int v)
{
	if (RoomGraphExtensions::FindNeighbourAbove(adGraph, v) >= 0) // Stairs room
		adGraph[v].label = RoomLabel::Stairs;
	else if (RoomGraphExtensions::FindNeighbourBelow(adGraph, v) >= 0) // First room on the floor
		adGraph[v].label = RoomLabel::Entrance;
	else
		adGraph[v].label = RoomLabel::Default;
}

void WorldGenerator::RoomTypesGenerator::SetDefaultNodeLabels(Graph<GeneratedRoom>& adGraph)
{
	// Respawn room
	adGraph[0].label = RoomLabel::Entrance;
	
	for (int i = 1; i < adGraph.Size() - 1; i++)
		SetDefaultNodeLabel(adGraph, i);
	
	// Roof room
	adGraph[adGraph.Size() - 1].label = RoomLabel::Roof;
}

void WorldGenerator::RoomTypesGenerator::GenerateFloorRoomTypes(Graph<GeneratedRoom>& adGraph)
{
	// Read grammar based on production files
	GraphGrammar<GeneratedRoom> grammar = GraphGrammarReader::ReadGraphGrammar<GeneratedRoom>(config.productionsDir());

	// Apply the grammar to the graph
	grammar.Apply(adGraph);
}

void RoomTypesGenerator::GenerateRoomTypes(Graph<GeneratedRoom>& adGraph)
{
	// Max z
	auto it = std::max_element(adGraph.nodes.begin(), adGraph.nodes.end(),
		[](const Node<GeneratedRoom>& n1, const Node<GeneratedRoom>& n2) {
			return n1.value->pos.z < n2.value->pos.z;
		});
	int max_floor = (*it).value->pos.z;

	// Perform for all floors excluding roof
	for (int z = 0; z < max_floor; z++)
	{
		std::vector<int> z_vertices = adGraph.GetVerticesIf([&](GeneratedRoom room)
			{return room.pos.z == z; }
		);

		// Get induced graph for the floor
		auto [G_z, map] = GraphUtils::GenerateInducedGraph(adGraph, z_vertices);

		GenerateFloorRoomTypes(G_z);

		for (int i = 0; i < G_z.Size(); i++)
			adGraph[map[i]].label = G_z[i].label;
	}
}
