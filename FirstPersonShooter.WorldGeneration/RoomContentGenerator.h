#pragma once
#include "RoomContentConfig.h"
#include "Graph.h"
#include "GeneratedRoom.h"
#include "PropLoader.hpp"

namespace WorldGenerator
{
	class RoomContentGenerator
	{
		RoomContentConfig config;
		std::vector<Prop> all_props;
	public:
		RoomContentGenerator(const RoomContentConfig& config)
			: config(config)
		{
		}
		
		void GenerateRoomContent(Graph<GeneratedRoom> &adGraph);

	private:
		void LoadPropsData();
		void GeneratePropsInRoom(Node<GeneratedRoom>& node);
		void GeneratePropInRoom(GeneratedRoom& room, Prop prop);
	};
}

