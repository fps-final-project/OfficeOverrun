#include "pch.h"
#include "RoomTypesGenerator.h"
#include "GraphGrammar.h";
#include "GraphGrammarReader.h";
#include "RoomLabel.h"
#include "RoomGraphExtensions.h"
#include "EnemyCountSelector.h"

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

void RoomTypesGenerator::GenerateRoomTypes(Graph<GeneratedRoom>& adGraph)
{
	// Read grammar based on production files
	GraphGrammar<GeneratedRoom> grammar = GraphGrammarReader::ReadGraphGrammar<GeneratedRoom>(config.productionsDir());

	// Apply the grammar to the graph
	grammar.Apply(adGraph);
}

void RoomTypesGenerator::GenerateEnemies(Graph<GeneratedRoom>& adGraph)
{
	for (int i = 0; i < adGraph.Size(); i++)
		adGraph[i].value->enemies = EnemyCountSelector::SelectEnemyCount((RoomLabel)adGraph[i].label);
}
