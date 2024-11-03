#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"

namespace WorldGenerator
{
	struct RoomSelectorArgs
	{
		Graph<GeneratedRoom> initialGraph;
		int startVertex;
		float roomDensity;
		float pathLengthCoeff;
		float edgeDensityCoeff;
		int floorCount;

		RoomSelectorArgs(Graph<GeneratedRoom> initialGraph) : initialGraph(initialGraph)
		{
		}
	};
}