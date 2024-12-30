#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	class RoomGraphExtensions
	{
	public:
		static int FindNeighbourAbove(Graph<GeneratedRoom>& graph, int v);
		static int FindNeighbourBelow(Graph<GeneratedRoom>& graph, int v);
	};
}