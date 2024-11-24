#pragma once

#include "Vector3.h"
#include "RoomLayout.h"
#include "RoomLayoutConfig.h"
#include "Graph.h"
#include "Utils.h"
#include "BinaryRoom.h"
#include "RoomLink.h"
#include "RoomSelector.h"

namespace WorldGenerator
{
	class RoomLayoutGenerator
	{
	private:
		RoomLayoutConfig config;
		Graph<GeneratedRoom> adGraph; // room adjacency graph
	public:
		RoomLayout layout;
		void GenerateRooms();
		void GenerateAdGraph();
		void SelectRooms();
		void GenerateRoomLinks();
		void GenerateLayoutFromAdGraph();
		RoomLayoutGenerator(RoomLayoutConfig config);
	};
}