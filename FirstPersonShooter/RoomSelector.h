#include "Graph.h"
#include "GeneratedRoom.h"
#include "RoomSelectorArgs.h"

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
		std::vector<int> neigh_G; // H[G] subgraph neighbourhood
		int s; // start vertex in H
		int floors;
		int s_G; // start vertex in G
		int N; // minimal number of rooms in the level
		float e_c; // edge probability coeff
		const int MAX_EDGE_WEIGHT = 1000; // maximum edge weight used in random path algorithm

		void RemoveUpDownEdges();
		static int FindNeighbourAbove(Graph<GeneratedRoom>& graph, int v);
		static int SelectVertexWithNeighbourAbove(Graph<GeneratedRoom>& graph, std::vector<int> vertices, int s);
		void UpdatePathWithFloor(std::vector<int>& P, int z);
		std::vector<int> GenerateRandomPath();
		void ConstructGFromPath(std::vector<int> P);
		void AddHVertexToG(int v);
		void RemoveDownUpEdges();
		void AddSpareVertices();
		void ComputeNeighbourhood();
		void UpdateNeighbourhood(int v);
		void AddEdgesAtRandom();
	public:

		RoomSelector(RoomSelectorArgs args);

		Graph<GeneratedRoom> SelectRooms();
	};
}

