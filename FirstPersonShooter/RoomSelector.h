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
		int s; // start vertex
		int N; // desired number of rooms in the level
		int P; // maximum length from s to e
		float e_c; // edge probability coeff

		void RemoveUpDownEdges();
	public:

		RoomSelector(RoomSelectorArgs args);

		Graph<GeneratedRoom> SelectRooms();
	};
}

