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
		int s; // start vertex
		int N; // desired number of rooms in the level
		int P; // maximum length from s to e
		float e_c; // edge probability coeff
		const int MIN_F = 3; // in pathfinding if possible traverse at least number of rooms before going up

		void RemoveUpDownEdges();
		std::vector<int> GenerateRandomPath();
		void RandomDfs(int v, std::vector<bool>& visited, std::vector<int>& path, int tr_f);
		void ConstructGFromPath(std::vector<int> P);
		void AddHVertexToG(int v);
		void RemoveDownUpEdges();
		void AddSpareVertices();
		void ComputeNeighbourhood();
		void UpdateNeighbourhood(int v);
	public:

		RoomSelector(RoomSelectorArgs args);

		Graph<GeneratedRoom> SelectRooms();
	};
}

