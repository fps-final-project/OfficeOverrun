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
		int s; // start vertex
		int N; // desired number of rooms in the level
		int P; // maximum length from s to e
		float e_c; // edge probability coeff
		const int MIN_F = 3; // in pathfinding if possible traverse at least number of rooms before going up

		void RemoveUpDownEdges();
		std::vector<int> GenerateRandomPath();
		void ConstructGFromPath(std::vector<int> P);
		void RandomDfs(int v, std::vector<bool>& visited, std::vector<int>& path, int tr_f);
	public:

		RoomSelector(RoomSelectorArgs args);

		Graph<GeneratedRoom> SelectRooms();
	};
}

