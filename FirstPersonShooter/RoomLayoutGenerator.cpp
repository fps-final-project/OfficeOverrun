#include "pch.h"
#include "RoomLayoutGenerator.h"
#include <algorithm>

using namespace WorldGenerator;

RoomLayoutGenerator::RoomLayoutGenerator(RoomLayoutConfig config) : config(config)
{
}

RoomLayout RoomLayoutGenerator::GenerateRooms()
{
	RoomLayout layout(config.mapSize);
	for (int floor = 0; floor < config.mapSize.z; floor++ )
	{
		BinaryRoom::MakeRoomsOnLayoutFloor(layout, floor);
	}
	// Add rooftop room
	GeneratedRoom roof;
	roof.pos = Vector3(0, 0, config.mapSize.z);
	roof.size = Vector3(config.mapSize.x, config.mapSize.y, 1);
	layout.rooms.push_back(roof);

	return layout;
}

Graph<GeneratedRoom> RoomLayoutGenerator::GenerateAdGraph(RoomLayout& layout)
{
	Graph<GeneratedRoom> adGraph(layout.rooms);
	for (int i = 0; i < adGraph.Size(); i++)
	{
		Node<GeneratedRoom>& node = adGraph[i];
		for (int j = i + 1; j < adGraph.Size(); j++)
		{
			if (node.value->ValidRoomLink(*adGraph[j].value))
			{
				adGraph.AddUndirectedEdge(i, j);
			}
		}
	}

	return adGraph;
}

void WorldGenerator::RoomLayoutGenerator::SelectRooms(Graph<GeneratedRoom>& adGraph)
{
	// Setup selector args
	RoomSelectorArgs args(adGraph);
	auto is_on_the_0_floor = [](Node<GeneratedRoom> node) { return node.value->IsZeroFloor(); };
	args.startVertex = std::distance(adGraph.nodes.begin(), 
		std::find_if(adGraph.nodes.begin(), adGraph.nodes.end(), is_on_the_0_floor)); // First index of room on zero floor
	args.roomDensity = config.roomDensity;
	args.edgeDensityCoeff = config.edgeDensityCoeff;
	args.floorCount = config.mapSize.z;

	RoomSelector selector(args);
	adGraph = selector.SelectRooms();
}

// The method removes some edges, spoils graph structure
void RoomLayoutGenerator::GenerateRoomLinks(Graph<GeneratedRoom>& adGraph)
{
	int n = adGraph.Size();
	for (int i = 0; i < n; i++) 
	{
		Node<GeneratedRoom>& node = adGraph[i];
		for (int j = i + 1; j < n; j++)
		{
			Node<GeneratedRoom>& neighbour = adGraph[j];
			if (!adGraph.HasEdge(i,j))
				continue;
			auto border = node.value->ComputeBorders(*neighbour.value);
			RoomLink link = RoomLink::MakeRoomLink(std::get<0>(border), std::get<1>(border));

			RoomLink outLink = link;
			outLink.linkedRoomIdx = j;

			RoomLink inLink = link;
			inLink.linkedRoomIdx = i;

			node.value->links.push_back(outLink);
			neighbour.value->links.push_back(inLink);
		}
	}
}

RoomLayout WorldGenerator::RoomLayoutGenerator::GenerateLayoutFromAdGraph(Graph<GeneratedRoom>& adGraph)
{
	std::vector<GeneratedRoom> newRooms;
	for (int i = 0; i < adGraph.Size(); i++)
	{
		newRooms.push_back(*adGraph[i].value);
	}

	RoomLayout layout(config.mapSize);
	layout.rooms = newRooms;

	return layout;
}
