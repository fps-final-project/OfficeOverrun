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
		
		void GenerateEnemies(Graph<GeneratedRoom>& adGraph);
		void GenerateProps(Graph<GeneratedRoom> &adGraph);

	private:
		void LoadPropsData();
		void GeneratePropsInRoom(Node<GeneratedRoom>& node);
		void GenerateGunsInRoom(Node<GeneratedRoom>& node);
		DirectX::XMFLOAT3 FindPropOrientation(const GeneratedRoom& room, Prop prop, DirectX::XMFLOAT2 prop_pos);
		PropInstance* GeneratePropForRoom(GeneratedRoom& room, Prop prop);
	};
}

