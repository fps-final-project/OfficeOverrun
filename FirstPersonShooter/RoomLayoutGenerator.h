#pragma once

#include "Vector3.h"
#include "RoomLayout.h"
#include "RoomLayoutConfig.h"
#include "Graph.h"

namespace WorldGenerator
{
	class RoomLayoutGenerator
	{
	private:
		RoomLayout layout;
		RoomLayoutConfig config;
		Graph<GeneratedRoom> adGraph; // room adjacency graph
	public:
		void GenerateRooms();
		void GenerateAdGraph();
		void SelectRooms();
		void GenerateRoomLinks();
		void GenerateLayoutFromAdGraph();
		RoomLayoutGenerator(RoomLayoutConfig config);
		RoomLayout& GetLayout();
	};
}