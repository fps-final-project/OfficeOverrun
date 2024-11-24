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
	class __declspec(dllexport) RoomLayoutGenerator
	{
	private:
		RoomLayoutConfig config;
	public:
		RoomLayout GenerateRooms();
		Graph<GeneratedRoom> GenerateAdGraph(RoomLayout& layout);
		void SelectRooms(Graph<GeneratedRoom>& adGraph);
		void GenerateRoomLinks(Graph<GeneratedRoom>& adGraph);
		RoomLayout GenerateLayoutFromAdGraph(Graph<GeneratedRoom>& adGraph);
		RoomLayoutGenerator(RoomLayoutConfig config);
	};
}