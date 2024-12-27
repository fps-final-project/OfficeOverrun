#include "pch.h"
#include "RoomTypesGenerator.h"
#include "GraphGrammar.h";
#include "GraphGrammarReader.h";

using namespace WorldGenerator;

void RoomTypesGenerator::GenerateRoomTypes(Graph<GeneratedRoom>& adGraph)
{
	// Read grammar based on production files
	GraphGrammar<GeneratedRoom> grammar = GraphGrammarReader::ReadGraphGrammar<GeneratedRoom>(config.productionsDir());

	// Apply the grammar to the graph
	grammar.Apply(adGraph);
}
