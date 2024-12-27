#include "pch.h"
#include "RoomTypesGenerator.h"
#include "GraphGrammar.h";
#include "GraphGrammarReader.h";

using namespace WorldGenerator;

void RoomTypesGenerator::GenerateRoomTypes(Graph<GeneratedRoom>& adGraph)
{
	GraphGrammar<GeneratedRoom> grammar = GraphGrammarReader::ReadGraphGrammar<GeneratedRoom>(PRODUCTIONS_DIR);
}
