#include "pch.h"
#include "RoomLayoutGenerator.h"
#include "Utils.h"
#include "BinaryRoom.h"
#include "RoomLink.h"

using namespace WorldGenerator;

RoomLayoutGenerator::RoomLayoutGenerator(RoomLayoutConfig config) : layout(RoomLayout(config.mapSize)), config(config)
{
}

RoomLayout RoomLayoutGenerator::Generate()
{
	// Step 1
	GenerateRooms();

	// Step 2
	GenerateAdGraph();

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
			if (node.value.IsAdjacent(adGraph.nodes[j].value))
			{
				node.neighbours.push_back(&adGraph.nodes[j]);
				adGraph.nodes[j].neighbours.push_back(&node);
			}
		}
	}
}

// The method removes some edges, spoils graph structure
void RoomLayoutGenerator::GenerateRoomLinks()
{
	int n = adGraph.nodes.size();
	for (int i = 0; i < n; i++) 
	{
		Node<GeneratedRoom>& node = adGraph.nodes[i];
		for (Node<GeneratedRoom>* neighbour : node.neighbours)
		{
			auto border = node.value.ComputeBorders(neighbour->value);
			auto link = RoomLink::MakeRoomLink(std::get<0>(border), std::get<1>(border), &(neighbour->value));
			node.value.links.push_back(link);
			neighbour->value.links.push_back(link);

			auto it = std::find(neighbour->neighbours.begin(), neighbour->neighbours.end(), &node);
			neighbour->neighbours.erase(it);
		}
	}
}
