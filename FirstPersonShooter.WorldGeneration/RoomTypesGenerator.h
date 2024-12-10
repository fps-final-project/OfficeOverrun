#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class RoomTypesGenerator
	{
	public:
		void GenerateRoomTypes(Graph<GeneratedRoom>& adGraph);
	private:
		void SetDefaultNodeLabels(Graph<GeneratedRoom> &adGraph);
	};
}

