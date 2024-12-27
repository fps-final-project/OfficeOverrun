#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class RoomTypesGenerator
	{
	private:
		const std::string PRODUCTIONS_DIR = R"(.\FirstPersonShooter_WorldGeneration\Grammar Configuration\Productions)";
		void SetDefaultNodeLabels(Graph<GeneratedRoom> &adGraph);
	public:
		void GenerateRoomTypes(Graph<GeneratedRoom>& adGraph);
	};
}

