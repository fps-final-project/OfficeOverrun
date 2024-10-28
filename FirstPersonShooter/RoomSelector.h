#include "Graph.h"
#include "GeneratedRoom.h"

#pragma once

namespace WorldGenerator
{
	class RoomSelector
	{
	private:
		Graph<GeneratedRoom> H;

		void RemoveUpDownEdges();
	public:

		RoomSelector(const Graph<GeneratedRoom> H)
			: H(H)
		{
		}

		Graph<GeneratedRoom> SelectRooms();
	};
}

