#include "pch.h"
#include "RoomGraphExtensions.h"

using namespace WorldGenerator;

// Returns index of the first neighbour above the vertex v or -1 if no such exists
int WorldGenerator::RoomGraphExtensions::FindNeighbourAbove(Graph<GeneratedRoom>& graph, int v)
{
	std::vector<int> neighbours = graph.GetNeighbours(v);
	auto it = std::find_if(neighbours.begin(), neighbours.end(), [&](int u)
		{
			return graph[u].value->IsAbove(*graph[v].value);
		}
	);
	return it != neighbours.end() ? *it : -1;
}

int WorldGenerator::RoomGraphExtensions::FindNeighbourBelow(Graph<GeneratedRoom>& graph, int v)
{
	std::vector<int> neighbours = graph.GetNeighbours(v);
	auto it = std::find_if(neighbours.begin(), neighbours.end(), [&](int u)
		{
			return graph[u].value->IsBelow(*graph[v].value);
		}
	);
	return it != neighbours.end() ? *it : -1;
}
