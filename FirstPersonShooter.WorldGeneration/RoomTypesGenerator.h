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
		void SetDefaultNodeLabels(Graph<GeneratedRoom> &adGraph);
	public:
		void GenerateRoomTypes(Graph<GeneratedRoom>& adGraph);
		RoomTypesGenerator(const RoomTypesConfig& config)
			: config(config) {};
	};
}

