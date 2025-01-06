#pragma once
#include "Graph.h"
#include "GeneratedRoom.h"
#include "RoomTypesConfig.h"

namespace WorldGenerator
{
	class RoomTypesGenerator
	{
	private:
		RoomTypesConfig config;
		void SetDefaultNodeLabel(Graph<GeneratedRoom> &adGraph, int v);
		void GenerateFloorRoomTypes(Graph<GeneratedRoom>& adGraph);
	public:
		void SetDefaultNodeLabels(Graph<GeneratedRoom> &adGraph);
		void GenerateRoomTypes(Graph<GeneratedRoom> &adGraph);
		RoomTypesGenerator(const RoomTypesConfig &config)
			: config(config) {};
	};
}

