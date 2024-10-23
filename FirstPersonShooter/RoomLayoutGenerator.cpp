#include "pch.h"
#include "RoomLayoutGenerator.h"
#include "Utils.h"
#include "BinaryRoom.h"
#include "RoomLink.h"

using namespace WorldGenerator;

RoomLayoutGenerator::RoomLayoutGenerator(RoomLayoutConfig config) : layout(RoomLayout(config.mapSize)), config(config)
{
}

RoomLayout& RoomLayoutGenerator::Generate()
{
	// Step 1
	GenerateRooms();

	// Step 2
	GenerateAdGraph();

	// Step 3
	

	// Step X - after graph operations
	GenerateRoomLinks();

	return layout;
}

void RoomLayoutGenerator::GenerateRooms()
{
	BinaryRoom rootRoom = BinaryRoom::GenerateRootRoom(layout.mapSize);
	rootRoom.Split(layout);
}

void RoomLayoutGenerator::GenerateAdGraph()
{
	adGraph = Graph<GeneratedRoom>(layout.rooms);
	for (int i = 0; i < adGraph.nodes.size(); i++)
	{
		Node<GeneratedRoom>& node = adGraph.nodes[i];
		for (int j = i + 1; j < adGraph.nodes.size(); j++)
		{
			if (node.value->IsAdjacent(*adGraph.nodes[j].value))
			{
				node.neighbours.push_back(j);
				adGraph.nodes[j].neighbours.push_back(i);
			}
		}
	}
}

void WorldGenerator::RoomLayoutGenerator::SelectRooms()
{
}

// The method removes some edges, spoils graph structure
void RoomLayoutGenerator::GenerateRoomLinks()
{
	int n = adGraph.nodes.size();
	for (int i = 0; i < n; i++) 
	{
		Node<GeneratedRoom>& node = adGraph.nodes[i];
		for (int j = i + 1; j < n; j++)
		{
			Node<GeneratedRoom>& neighbour = adGraph.nodes[j];
			if (!node.IsConnectedTo(j))
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
