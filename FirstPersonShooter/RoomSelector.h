#include "Graph.h"
#include "GeneratedRoom.h"
#include "RoomSelectorArgs.h"
#include "RNG.h"
#include "WeightedGraph.h"
#include "GraphUtils.h"
#include "PathFinding.h"
#include <cmath>
#include <algorithm>
#include <utility>

#pragma once

namespace WorldGenerator
{
	// Class performing subgraph selection algorithm
	class RoomSelector
	{
	private:
		Graph<GeneratedRoom> H; // initial graph
		Graph<GeneratedRoom> G; // generated graph
		std::vector<int> G_H_map; // maps G vertices to H vertices
		std::vector<int> H_G_map; // maps H vertices to G vertices
		int s; // start vertex in H
		int floors;
		int s_G; // start vertex in G
		int t_G; // target vertex in G
		int N; // minimal number of rooms in the level
		float e_c; // edge probability coeff
		const int MAX_EDGE_WEIGHT = 1000; // maximum edge weight used in random path algorithm

		static int SelectVertexWithNeighbourAbove(Graph<GeneratedRoom>& graph, std::vector<int> vertices, int s);
		void UpdatePathWithFloor(std::vector<int>& P, int z);
		std::vector<int> GenerateRandomPath();
		void ConstructGFromPath(std::vector<int> P);
		void AddHVertexToG(int v);
		void RemoveStairsEdges();
		void AddSpareVertices();
		void AddRooftopVertex();
		std::vector<int> ComputeNeighbourhood();
		void UpdateNeighbourhood(std::vector<int> &neigh_G, int v);
		void AddEdgesAtRandom();
	public:

		RoomSelector(RoomSelectorArgs args);

		Graph<GeneratedRoom> SelectRooms();
	};
}

